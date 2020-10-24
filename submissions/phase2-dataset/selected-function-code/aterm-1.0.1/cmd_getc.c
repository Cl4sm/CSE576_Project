unsigned char
cmd_getc(void)
{
#define TIMEOUT_USEC	5000
    static short    refreshed = 0;
    fd_set          readfds;
    int             retval;

	struct timeval  value;
#if 0
#ifdef __CYGWIN32__
    struct timeval  value;
#else
    struct itimerval value;
#endif
#endif

/* If there have been a lot of new lines, then update the screen
 * What the heck I'll cheat and only refresh less than every page-full.
 * the number of pages between refreshes is refresh_limit, which
 * is incremented here because we must be doing flat-out scrolling.
 *
 * refreshing should be correct for small scrolls, because of the
 * time-out */
    if (refresh_count >= (refresh_limit * (TermWin.nrow - 1))) {
		if (refresh_limit < REFRESH_PERIOD)
	    	refresh_limit++;
		refresh_count = 0;
		refreshed = 1;
		scr_refresh(refresh_type);
    }
/* characters already read in */
    if (cmdbuf_ptr < cmdbuf_endp)
	goto Return_Char;

    for (;;) {
		struct timeval *timeout = &value;

	if (v_bufstr < v_bufptr)	/* output any pending chars */
	    tt_write(NULL, 0);
	while (XPending(Xdisplay)) {	/* process pending X events */
	    refreshed = 0;
#ifdef USE_XIM
            XProcessEvent(Xdisplay);
#else
		{		
			XEvent          ev;
			XNextEvent(Xdisplay, &ev);
/*fprintf( stderr, "%s:%d Received event %d\n", __FUNCTION__, __LINE__, ev.type );*/
	    	process_x_event(&ev);
	    }
#endif
	/* in case button actions pushed chars to cmdbuf */
	    if (cmdbuf_ptr < cmdbuf_endp)
		goto Return_Char;
	}
#ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
	if (scrollbar_isUp()) {
	    if (!scroll_arrow_delay-- && scr_page(UP, 1)) {
		scroll_arrow_delay = SCROLLBAR_CONTINUOUS_DELAY;
		refreshed = 0;
		refresh_type |= SMOOTH_REFRESH;
	    }
	} else if (scrollbar_isDn()) {
	    if (!scroll_arrow_delay-- && scr_page(DN, 1)) {
		scroll_arrow_delay = SCROLLBAR_CONTINUOUS_DELAY;
		refreshed = 0;
		refresh_type |= SMOOTH_REFRESH;
	    }
	}
#endif				/* NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING */

    /* Nothing to do! */
	FD_ZERO(&readfds);
	FD_SET(cmd_fd, &readfds);
	FD_SET(Xfd, &readfds);

	if( refreshed && last_update_background_request_sec == 0
#ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
			  && !(scrollbar_isUpDn())
#endif
		)
			timeout = NULL ;
	else if( !refreshed )
	{
		timeout->tv_usec = TIMEOUT_USEC;
		timeout->tv_sec = 0;
	}else if( last_update_background_request_sec ) 
	{
		time_t curr_t_sec, curr_t_usec, wait_usec = 0 ;	  
		timer_get_time (&curr_t_sec, &curr_t_usec);
		if( curr_t_sec == last_update_background_request_sec ) 
		{
			if( curr_t_usec < last_update_background_request_usec ) 
				wait_usec = last_update_background_request_usec - curr_t_usec ; 
		}else if( curr_t_sec < last_update_background_request_sec ) 
		{
			wait_usec = (last_update_background_request_sec - curr_t_sec)*1000000 ;	
			wait_usec += last_update_background_request_usec ;
			wait_usec -= curr_t_usec ;
		}	 
		if( wait_usec == 0 ) 
			wait_usec = TIMEOUT_USEC ;
		timeout->tv_usec = wait_usec;
		timeout->tv_sec = 0 ; /*UPDATE_BACKGROUND_TIMEOUT_SEC;*/
	}	 
/*fprintf( stderr, "%s:%d select(%d, cmd_fd = %d, Xfd = %d, %p)...", __FUNCTION__, __LINE__, num_fds, cmd_fd,Xfd ,timeout ); */
	retval = select(max(cmd_fd,Xfd)+1, &readfds, NULL, NULL, timeout );
/*	fprintf( stderr, "Done(retval = %d).\n", retval); */
    /* See if we can read from the application */
	if (FD_ISSET(cmd_fd, &readfds)) {
	    int             n;
	    unsigned int    count;

	    cmdbuf_ptr = cmdbuf_endp = cmdbuf_base;
	    for (count = BUFSIZ; count; count -= n, cmdbuf_endp += n)
		if ((n = read(cmd_fd, cmdbuf_endp, count)) > 0)
		    continue;
		else if (n == 0 || (n < 0 && errno == EAGAIN))
		    break;
		else {
#if !defined(HAVE_ATEXIT) && !defined(__sun__)
		    clean_exit();
#endif
		    exit(EXIT_SUCCESS);
		}
	/* some characters read in */
	    if (count != BUFSIZ)
		goto Return_Char;
	}
    /* select statement timed out - better update the screen */
	if (retval == 0) 
	{
	    	refresh_count = 0;
	    	refresh_limit = 1;
			if( last_update_background_request_sec > 0 )
			{
				time_t curr_t_sec, curr_t_usec;	  
				timer_get_time (&curr_t_sec, &curr_t_usec);
				if( (	last_update_background_request_sec == curr_t_sec && 
						last_update_background_request_usec < curr_t_usec) ||
					last_update_background_request_sec < curr_t_sec || 
					last_update_background_request_sec-1 > curr_t_sec )
				{
					/* TODO update background pixmap */						
					RenderPixmap(1);
					refresh_transparent_scrollbar();
	        		scr_clear();
					scr_touch();
					refreshed = True ;
				}
			}
	    	if (!refreshed) 
			{
				refreshed = 1;
				scr_refresh(refresh_type);
				scrollbar_show(1);
#ifdef USE_XIM
                IMSendSpot();
#endif
		    }
		}
    }
    /* NOTREACHED */
    return 0;

  Return_Char:
    refreshed = 0;
    return (*cmdbuf_ptr++);
}
