unsigned char
rxvt_cmd_getc(rxvt_t *r, int* p_page)
{
    int		    selpage = *p_page, retpage;
    fd_set	    readfds;
    int		    quick_timeout, select_res;
#ifdef POINTER_BLANK
    int		    want_motion_time = 0;
#endif
#ifdef CURSOR_BLINK
    int		    want_keypress_time = 0;
#endif
#if defined(POINTER_BLANK) || defined(CURSOR_BLINK) || defined(TRANSPARENT)
    struct timeval  tp;
#endif
    struct timeval  value;
    struct rxvt_hidden *h = r->h;
    register int    i;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "Entering rxvt_cmd_getc on page %d\n", *p_page));


    /* loop until we can return something */
    for(;;)
    {
	/* check for inactivity */
	for (i = 0; i <= LTAB(r); i ++)
	    rxvt_monitor_tab(r,i);

#if defined(POINTER_BLANK) || defined(CURSOR_BLINK) || defined(TRANSPARENT)
	/* presume == 0 implies time not yet retrieved */
	tp.tv_sec = tp.tv_usec = 0;
#endif	/* POINTER_BLANK || CURSOR_BLINK || TRANSPARENT*/
#ifdef CURSOR_BLINK
	want_keypress_time = 0;
#endif	/* CURSOR_BLINK */
#ifdef POINTER_BLANK
	if (ISSET_OPTION(r, Opt_pointerBlank))
	    want_motion_time = 0;
#endif	/* POINTER_BLANK */


	if( selpage == -1 )
	{
	    /* Process all pending X events */
	    while( XPending(r->Xdisplay) )
	    {
		XEvent	  xev;

		XNextEvent(r->Xdisplay, &xev);


#ifdef CURSOR_BLINK
		if (ISSET_OPTION(r, Opt_cursorBlink) &&
		    KeyPress == xev.type)
		{
		    if (h->hidden_cursor)
		    {
			rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** hide cursor on keypress\n"));
			h->hidden_cursor = 0;
			AVTS(r)->want_refresh = 1;
		    }
		    want_keypress_time = 1;
		}
#endif	/* CURSOR_BLINK */

#ifdef POINTER_BLANK
		if (ISSET_OPTION(r, Opt_pointerBlank) &&
		    (h->pointerBlankDelay > 0))
		{
		    if (MotionNotify == xev.type ||
			ButtonPress == xev.type ||
			ButtonRelease == xev.type )
		    {
			/* only work for current active tab */
			if (AVTS(r)->hidden_pointer)
			    rxvt_pointer_unblank(r, ATAB(r));
			want_motion_time = 1;
		    }
		    /* only work for current active tab */
		    if (KeyPress == xev.type && !AVTS(r)->hidden_pointer)
			rxvt_pointer_blank(r, ATAB(r));
		}
#endif	/* POINTER_BLANK */

#ifdef USE_XIM
		if (NOT_NULL(r->h->Input_Context))
		{
		    if (!XFilterEvent(&xev, xev.xany.window))
			rxvt_process_x_event(r, &xev);
		    h->event_type = xev.type;
		}
		else
#endif	/* USE_XIM */
		{
		    rxvt_process_x_event(r, &xev);
		}
	    }   /* while ((XPending(r->Xdisplay)) */
	} /* if( selpage == -1 ) */

	else if( !PVTS(r, selpage)->dead && XPending( r->Xdisplay ) )
	{
	    /*
	     * selpage != -1 on an alive tab, and X events are pending. If this
	     * tab produces lots of output, it could potentially choke
	     * everything else. Thus we return a failure, so the caller will
	     * rxvt_set_escfail() and fall back to rxvt_main_loop(). We will be
	     * called again with selpage == -1, when we can process X events.
	     */
	    *p_page = -1;
	    return 0;
	}



	/*
	 * We are done processing our X events. Check to see if we have any data
	 * pending in our input buffer.
	 */
	if( selpage != -1 && rxvt_cmdbuf_has_input(r, selpage) )
	    return *(PVTS(r, selpage)->cmdbuf_ptr)++;

	if( selpage == -1 && -1 != (retpage = rxvt_find_cmd_child (r)) )
	{
	    /*
	     * In case -1 == selpage we are free to return data from any tab we
	     * choose. Note, that rxvt_find_cmd_child() will favor returning the
	     * active tab.
	     */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_find_cmd_child: find %d\n", retpage));

	    *p_page = retpage;
	    return *(PVTS(r, *p_page)->cmdbuf_ptr)++;
	}


	/*
	 * The command input buffer is empty and we have no pending X events.
	 * We call select() to wait until some data is available.
	 */
#ifdef CURSOR_BLINK
	if (want_keypress_time)
	{
	    /* reset last cursor change time on keypress event */
	    (void) gettimeofday (&tp, NULL);
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "** init cursor time on keypress\n"));
	    h->lastcursorchange.tv_sec = tp.tv_sec;
	    h->lastcursorchange.tv_usec = tp.tv_usec;
	}
#endif	/* CURSOR_BLINK */

#ifdef POINTER_BLANK
	if (ISSET_OPTION(r, Opt_pointerBlank) && want_motion_time)
	{
	    (void) gettimeofday (&tp, NULL);
	    h->lastmotion.tv_sec = tp.tv_sec;
	    h->lastmotion.tv_usec = tp.tv_usec;
	}
#endif	/* POINTER_BLANK */

	quick_timeout = rxvt_check_quick_timeout (r);
	quick_timeout = rxvt_adjust_quick_timeout (r, quick_timeout, &value);

	/* Now begin to read in from children's file descriptors */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Waiting for %lumu for child\n", 
		      quick_timeout ? value.tv_sec * 1000000LU + value.tv_usec : ULONG_MAX));


	/* Prepare to read in from children's file descriptors */
	FD_ZERO(&readfds);
	FD_SET(r->Xfd, &readfds);

	for (i = 0; i <= LTAB(r); i ++)
	{
	    /* remember to skip held childrens */
	    if ( PVTS(r, i)->hold > 1 )
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    " not listening on vt[%d].cmd_fd (dead)\n", i));
		continue;
	    }
	    else if ( PVTS(r, i)->gotEIO )
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    " not listening on vt[%d].cmd_fd (EIO)\n", i));
		PVTS(r, i)->gotEIO = 0;
		continue;
	    }

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			" listening on vt[%d].cmd_fd = %d\n",
			i, PVTS(r, i)->cmd_fd));
	    FD_SET(PVTS(r, i)->cmd_fd, &readfds);

	    /* Write out any pending output to child */
	    if( PVTS(r, i)->v_bufstr < PVTS(r, i)->v_bufptr )
		rxvt_tt_write(r, i, NULL, 0);
	}

#ifdef HAVE_X11_SM_SMLIB_H
	if (-1 != r->TermWin.ice_fd)
	    FD_SET(r->TermWin.ice_fd, &readfds);
#endif

#ifdef USE_FIFO
	if( r->fifo_fd != -1 )
	    FD_SET( r->fifo_fd, &readfds );
#endif

	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
		    "Calling select( num_fds=%d, timeout=%06du, &readfds)",
		    r->num_fds,
		    quick_timeout ? value.tv_sec * 1000000 + value.tv_usec : -1
		    ));
	select_res = select( r->num_fds, &readfds, NULL, NULL,
			(quick_timeout ? &value : NULL) );
	rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
		    "done (timeout %06du). Return %d\n",
		    quick_timeout ? value.tv_sec * 1000000 + value.tv_usec : -1,
		    select_res ));

	if( select_res > 0 )
	{
	    /* Select succeeded. Check if we have new Xevents first. */
	    if( 0 && selpage == -1 && XPending( r->Xdisplay ) > 25)
	    {
		rxvt_dbgmsg(( DBG_DEBUG, DBG_COMMAND,
			"%d xevents to processes. Continuing\n",
			XPending( r->Xdisplay ) ));
		continue;
	    }

	    /* Read whatever input we can from child fd's*/
	    rxvt_process_children_cmdfd (r, &readfds);

#ifdef HAVE_X11_SM_SMLIB_H
	    /*
	     * ICE file descriptor must be processed after we process all file
	     * descriptors of children. Otherwise, if a child exit,
	     * IceProcessMessages may hang and make the entire terminal
	     * unresponsive.
	     */
	    if(
		 -1 != r->TermWin.ice_fd &&
		 FD_ISSET (r->TermWin.ice_fd, &readfds)
	      )
		rxvt_process_ice_msgs (r);
#endif

#ifdef USE_FIFO /* {{{ Execute macros read from the fifo */
	    if( -1 != r->fifo_fd  && FD_ISSET(r->fifo_fd, &readfds))
	    {
		ssize_t	len;
		int	nbytes;

		nbytes = sizeof(r->fifo_buf) - (r->fbuf_ptr - r->fifo_buf) - 1;
		assert( nbytes > 0 );
		
		errno = 0;
		len = read( r->fifo_fd, r->fbuf_ptr, nbytes );
		
		if( errno )
		{
		    rxvt_msg (DBG_ERROR, DBG_COMMAND, "Error: reading fifo %s", strerror (errno));
		}

		if( len == 0 )
		{
		    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Reopening fifo %s\n", r->fifo_name ));
		    close( r->fifo_fd );
		    r->fifo_fd = open( r->fifo_name, O_RDONLY|O_NDELAY );
		    rxvt_adjust_fd_number( r );

		    /* Flush the fifo buffer */
		    r->fbuf_ptr = r->fifo_buf;
		}

		else if( len > 0 )
		{
		    char	astr[FIFO_BUF_SIZE];
		    char	*fptr = r->fifo_buf,
				*aptr;
		    action_t    action;

		    SET_NULL( action.str );

		    r->fbuf_ptr += len;	    /* Point to end of fifo_buf */

		    for(;;)
		    {
			aptr = astr;
			while( fptr < r->fbuf_ptr && *fptr && *fptr != '\n' )
			    *(aptr++) = *(fptr++);

			if( fptr < r->fbuf_ptr && *fptr == '\n' )
			{
			    /* Got complete action. Execute it */
			    *aptr = 0;
			    if( rxvt_set_action( &action, astr ) )
				rxvt_dispatch_action( r, &action, NULL );

			    fptr++; /* Advance to next char */
			}

			else
			{
			    /*
			     * Incomplete action. Copy it to the fifo buffer and
			     * break out
			     */
			    MEMCPY( r->fifo_buf, astr, aptr - astr );
			    r->fbuf_ptr = r->fifo_buf + (aptr - astr);
			    break;
			}

		    }

		    rxvt_free( action.str );
		}
	    }
#endif/*USE_FIFO}}}*/

	    /*
	     * Now figure out if we have something to return.
	     */
	    if( selpage != -1 && rxvt_cmdbuf_has_input(r, selpage) )
		return *(PVTS(r, selpage)->cmdbuf_ptr)++;

	    /* No input from specified child. Try others. */
	    else if( (retpage = rxvt_find_cmd_child (r)) != -1 )
	    {
		if( selpage != -1 && selpage != retpage )
		{
		    /*
		     * Specified child has nothing to return, but some other
		     * child has data to return. We set retpage = -1, and return
		     * 0.
		     */
		    *p_page = -1;
		    return '\0';
		}

		else
		{
		    /* No child specified, and we have input from some child */
		    *p_page = retpage;
		    return *(PVTS(r, retpage)->cmdbuf_ptr)++;
		}
	    } /* else if( (retpage = rxvt_find_cmd_child (r)) != -1 ) */
	} /* if( select_res >= 0 ) */

	/*
	 * If we get here, we either have a select() error, or no tabs had any
	 * input. Check to see if something died.
	 */
	if( r->ndead_childs || select_res == -1 )
	    rxvt_mark_dead_childs( r );

	if( r->cleanDeadChilds )
	{
	    /* Ok. Something died. */
	    *p_page = -1;
	    return '\0';
	} /* if( r->cleanDeadChilds ) */


	/*
	 * Nothing to return. Screen refresh, and call select() again.
	 */
	rxvt_refresh_vtscr_if_needed( r );

    }	/* for(;;) */

    /* NOT REACHED */
}