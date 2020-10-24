int
rxvt_dispatch_action( rxvt_t *r, action_t *action, XEvent *ev)
{
    const int	maxLen = 1024;
    char	expstr[ maxLen ];
    char	*astr;
    int		alen,
		retval = 1;	    /* Succeed by default */

    if( IS_NULL( action->str ) )
    {
	SET_NULL( astr );
	alen = 0;
    }
    else
    {
	/* % interpolate the action string */
	astr = expstr;
	alen = rxvt_percent_interpolate( r, ATAB(r), (char *) action->str,
		action->len, astr, maxLen );
    }


    switch( action->type )
    {
	case MacroFnEsc:
	    /* Send action to rxvt */
	    if( NOT_NULL( astr ) && alen > 1 )
		rxvt_cmd_write( r, ATAB(r), (unsigned char*) astr, alen - 1);
	    else
	    {
		rxvt_msg (DBG_ERROR, DBG_MACROS,  "Macro %s requires argument.",
			macroNames[action->type] );
		retval = -1;
	    }
	    break;

	case MacroFnStr:
	    /* Send action to child process */
	    if( NOT_NULL( astr ) && alen > 1 )
		rxvt_tt_write( r, ATAB(r), (unsigned char*) astr, alen - 1);
	    else
	    {
		rxvt_msg (DBG_ERROR, DBG_MACROS,  "Macro %s requires argument.",
			macroNames[action->type] );
		retval = -1;
	    }
	    break;

	case MacroFnNewTab:
	    if (NOT_NULL(astr))
	    {
		/*
		 * If the first word is quoted, use that as the title. Don't be
		 * fancy and check for nested quotes. That's probably
		 * unnecessary.
		 *
		 * Everything after the first quoted word is the command. If
		 * command starts with "!", then the shell is exec'ed before
		 * running command.
		 */

		const int   MaxMacroTitle = 80;	/* Longest title we will have */
		char	    titlestring[MaxMacroTitle];
		char	    *command = (char *) astr;
		char	    *title = NULL;

		int	    profile = 0;

		/* See if a profile is specified */
		if( *command == '-' )
		{
		    char *pnum_end;
		    profile = strtoul( ++command, &pnum_end, 0 );

		    if( profile < 0 || profile >= MAX_PROFILES )
			profile = AVTS(r)->profileNum;

		    /* Skip spaces */
		    command = pnum_end;
		    while( isspace( *command ) ) command++;
		}

		/* See if a title is specified */
		if( *command == '"' )
		{
		    int i;

		    /* Copy everything until first " into title */
		    for(
			  i=0, command++;
			  i < MaxMacroTitle - 2 && *command && *command != '"';
			  i++, command++
		       )
			titlestring[i] = *command;
		    titlestring[i] = '\0'; /* Null terminate title */
		    title = titlestring;

		    /* Skip spaces after title */
		    if( *command ) command++;
		    while( isspace( *command ) ) command++;
		}

		/* Add page */
		rxvt_append_page( r, profile, title,
				    *command ? command : NULL );
	    }
	    else
		rxvt_append_page( r, 0, NULL, NULL );

	    break;

	case MacroFnExec:
	    if( NOT_NULL( astr ) )
		retval = rxvt_async_exec( r, astr ) ? 1 : -1;

	    else
	    {
		rxvt_msg (DBG_ERROR, DBG_MACROS,  "Macro %s requires argument.",
			macroNames[action->type] );
		retval = -1;
	    }

	    break;

	case MacroFnClose:
	    if( alen > 0 && *(astr) )
	    {
		/* Close tab specified by str */
		int tabno = atoi( (char*) astr) - 1;

		if( tabno == -1 ) tabno = ATAB(r);

		if (
		      tabno >=0 && tabno <=LTAB(r)
		      && (
			   NOTSET_OPTION(r, Opt2_protectSecondary)
			   || PVTS(r, tabno)->current_screen == PRIMARY
			 )
		   )
		{
		    rxvt_kill_page (r, tabno);
		}
		else
		    retval = -1;
	    }
	    else
		rxvt_exit_request( r );

	    break;

	case MacroFnGoto:
	{
	    /* Goto tab in position astr */
	    int tabno;
	    
	    if (NOT_NULL(astr) && *(astr) )
	    {
		tabno = atoi( (char*) astr );

		if( *(astr)  == '+' || *(astr) == '-' )
		{
		    /*
		     * Relative movement of tabs
		     */
		    tabno += ATAB(r);

		    /* Wrap around */
		    tabno = tabno % (LTAB(r) + 1);
		    if( tabno < 0 ) tabno += LTAB(r) + 1;
		}
		else if( tabno == 0 )
		{
		    /*
		     * Previous active tab
		     */
		    tabno = PTAB(r);
		}
		else if( --tabno > LTAB(r) )
		{
		    /*
		     * Absolute tab number. If we're too far to the right,
		     * activate the last tab.
		     */
		    tabno = LTAB(r);
		}
	    }
	    else tabno = PTAB(r);

	    rxvt_activate_page( r, tabno);
	    break;
	}

	case MacroFnMove:
	    /* Move active tab to position in astr */
	    if( alen > 0 && *(astr) )
	    {
		short tabno = atoi( (char*) astr );

		if( *(astr) == '+' || *(astr) == '-' )
		    rxvt_tabbar_move_tab( r, tabno + ATAB(r));
		else
		    rxvt_tabbar_move_tab( r, tabno-1 );
	    }

	    else
		retval = -1;
	    break;

	case MacroFnScroll:
	    /* Scroll by an amount specified in astr */
	    if( alen > 1 )
	    {
		int		amount	    = abs( atoi( (char*) astr ));
		enum page_dirn	direction   = (*(astr) == '-' ? UP : DN);

		rxvt_dbgmsg ((DBG_DEBUG, DBG_MACROS, "astr: '%s', alen: %d\n", astr, alen));

		if( tolower( astr[ alen - 2] ) == 'p' )
		    /* scroll pages */
		    amount *=
#ifdef PAGING_CONTEXT_LINES
				r->TermWin.nrow - PAGING_CONTEXT_LINES
#else
				r->TermWin.nrow * 4 / 5
#endif
		    ;

		rxvt_scr_page( r, ATAB(r), direction, amount);
#  ifdef HAVE_SCROLLBARS
		rxvt_scrollbar_update(r, 1);
#  endif    /* HAVE_SCROLLBARS */
	    }
	    break;

#if 0
	case MacroFnCopy:
#endif
	case MacroFnPaste:
	{
	    int sel = XA_PRIMARY;

	    if (NOT_NULL(ev))
	    {
		if( NOT_NULL(astr) && *astr )
		{
		    if(strcmp ("PRIMARY", astr) == 0)
			sel=XA_PRIMARY;
		    else if (strcmp ("SECONDARY", astr) == 0)
		      sel=XA_SECONDARY;
		    else if (strcmp ("CLIPBOARD", astr) == 0)
		      sel=XA_CLIPBOARD;
		    else
		      break;
		    rxvt_selection_request_by_sel( r, ATAB(r), ev->xkey.time,
			    0, 0, sel);
		}
		else
		    rxvt_selection_request (r, ATAB(r), ev->xkey.time, 0, 0);
	    }
	    else
	    {
		retval = -1;
	    }
	    break;
	}

	case MacroFnPasteFile:
	{
	    if (NOT_NULL(ev))
	    {
	        if( NOT_NULL(astr) && *astr )
		    rxvt_paste_file (r, ATAB(r), ev->xkey.time, 0, 0, astr);
	        else
		    break;
	    }
	    else
		retval = -1;

	   break;
	}

	case MacroFnMonitorTab:
	{
	    if (NOT_NULL(ev))
	    {
		if( NOT_NULL(astr) && *astr )
		{
		    short doit = 0;

		    /* which monitoring type do we need ? */
		    if(strcmp ("ACTIVITY", astr) == 0)
		    {
		      AVTS(r)->monitor_tab = 
			  (AVTS(r)->monitor_tab == TAB_MON_ACTIVITY ) ? TAB_MON_OFF : TAB_MON_ACTIVITY;
		      doit = 1;
		      rxvt_msg (DBG_INFO, DBG_MACROS,  "Macro %s ACTIVITY : activity monitoring request on tab %i.",
			macroNames[action->type], AVTS(r)->vts_idx );
		    }
		    else if (strcmp ("INACTIVITY", astr) == 0)
		    {
		      AVTS(r)->monitor_tab = 
			  (AVTS(r)->monitor_tab == TAB_MON_INACTIVITY) ? TAB_MON_OFF : TAB_MON_INACTIVITY;
		      doit = 1;
		      rxvt_msg (DBG_INFO, DBG_MACROS,  "Macro %s INACTIVITY : inactivity monitoring request on tab %i.",
		 	 macroNames[action->type], AVTS(r)->vts_idx );
		    }
		    else if (strcmp ("AUTO", astr) == 0)
		    {
		      AVTS(r)->monitor_tab = 
			  (AVTS(r)->monitor_tab == TAB_MON_AUTO) ? TAB_MON_OFF : TAB_MON_AUTO;
		      doit = 1;
		      rxvt_msg (DBG_INFO, DBG_MACROS,  
			 "Macro %s AUTO : request for automatic (in-)activity monitoring on tab %i.",
		 	 macroNames[action->type], AVTS(r)->vts_idx );
		    }
		    else
		    {
		      rxvt_msg (DBG_INFO, DBG_MACROS,  "Macro %s requires argument or invalid argument provided.",
		 	 macroNames[action->type] );
		      break;
		    }
		    /* activating/deactivating the macro */
		    if (doit != 0)
		    {
		      if (AVTS(r)->monitor_tab == TAB_MON_OFF )
		      {
			  rxvt_msg (DBG_INFO, DBG_MACROS,  "Macro %s was already active, deactivating previous macro call.",
			     macroNames[action->type] );
		      }else
		      {
			  AVTS(r)->monitor_nbytes_read = 0;
			  gettimeofday( &AVTS(r)->monitor_start , NULL);
		      }
		    }
		}
	    }
	    else
	    {
		retval = -1;
	    }
	    break;
	}

	case MacroFnToggleSubwin:
	    rxvt_toggle_subwin( r, (unsigned char*) astr);
	    break;

	case MacroFnFont:
	{
	    const int MaxFontLen = 8;	/* Only need space for "#+xx" */

	    char fontname[MaxFontLen];
	    if( alen >= MaxFontLen - 1 ) break;	/* Remember that
						   alen includes
						   the trailing null
						   char */

	    fontname[0] = FONT_CMD;		/* Internal prefix */
	    STRNCPY( fontname + 1, astr, MaxFontLen - 1);
	    fontname[MaxFontLen - 1] = '\0';	/* Null terminate */

	    rxvt_resize_on_font( r, fontname );
	    break;
	}

	case MacroFnToggleVeryBold:
	    TOGGLE_OPTION( r, Opt2_veryBold );

	    rxvt_scr_touch (r, ATAB(r), True);
	    break;

	case MacroFnToggleBoldColors:
	    TOGGLE_OPTION( r, Opt2_boldColors );

	    rxvt_scr_touch (r, ATAB(r), True);
	    break;

	case MacroFnToggleVeryBright:
	    TOGGLE_OPTION( r, Opt_veryBright );

	    rxvt_scr_touch (r, ATAB(r), True);
	    break;

	case MacroFnToggleTransp:
#ifdef TRANSPARENT
	    rxvt_toggle_transparency(r);
#else
	    retval = -1;
#endif
	    break;

	case MacroFnToggleBcst:
	    if( NOT_NULL(astr) && *astr )
	    {
		long state = strtol( astr, NULL, 0 );

		switch(state)
		{
		    case 1:
			SET_OPTION( r, Opt2_broadcast );
			break;

		    case 0:
			UNSET_OPTION( r, Opt2_broadcast );
			break;

		    case -1:
			TOGGLE_OPTION( r, Opt2_broadcast );
			break;

		    default:
			rxvt_msg( DBG_ERROR, DBG_MACROS,
				"Badly formed argument '%s' to %s\n",
				astr, macroNames[action->type] );
			retval = -1;
			break;
		}
	    }
	    else
		TOGGLE_OPTION(r, Opt2_broadcast);
	    break;

	case MacroFnToggleHold:
	    if( NOT_NULL(astr) && *astr )
	    {
		/* Set the hold option for this tab */
		char		op = *astr++;
		unsigned long	holdMask;
		
		holdMask = strtoul( astr, NULL, 0 );
		switch( op )
		{
		    case '+':
			AVTS(r)->holdOption |= holdMask;
			break;

		    case '-':
			AVTS(r)->holdOption &= ~holdMask;
			break;

		    case '!':
			AVTS(r)->holdOption ^= holdMask;
			break;

		    default:
			rxvt_msg (DBG_ERROR, DBG_MACROS,  "Badly formed argument '%s' to %s\n",
				astr, macroNames[action->type] );
			retval = -1;
			break;
		}

		/* Remove the ATAB if it no longer needs to be held */
		if(
		     AVTS(r)->dead && AVTS(r)->hold > 1 &&
		     !SHOULD_HOLD( r, ATAB(r) )
		  )
		    rxvt_remove_page( r, ATAB(r) );
	    }

	    else
	    {
		/*
		 * Behaviour almost compatible with mrxvt-0.5.1: Just get rid of
		 * all held tabs.
		 */
		int k;

		for (k = LTAB(r); k>= 0; k --)
		    if (PVTS(r, k)->dead && PVTS(r, k)->hold > 1)
			rxvt_remove_page (r, k);
	    }

	    break;

	case MacroFnToggleFullscreen:
	    ewmh_message( r->Xdisplay, XROOT, r->TermWin.parent,
		XInternAtom( r->Xdisplay, "_NET_WM_STATE", True),
		_NET_WM_STATE_TOGGLE,
		XInternAtom( r->Xdisplay, "_NET_WM_STATE_FULLSCREEN", True),
		0, 0, 0 );
	    break;

	case MacroFnRaise:
	    ewmh_message( r->Xdisplay, XROOT, r->TermWin.parent,
		XInternAtom( r->Xdisplay, "_NET_ACTIVE_WINDOW", True),
		1, 0 /*timestamp?*/, r->TermWin.parent, 0, 0 );
	    break;


	case MacroFnSetTitle:
	    if (NOT_NULL(astr))
		rxvt_tabbar_set_title( r, ATAB(r),
			(unsigned char*) astr);
	    else if (NOT_NULL(r->selection.text))
		rxvt_tabbar_set_title( r, ATAB(r),
			(const unsigned char TAINTED*) r->selection.text);
	    else
		retval = -1;
	    break;

#ifdef USE_FIFO
	case MacroFnUseFifo:
	    if( NOT_NULL( astr ) )
	    {
		int state = strtol( astr, NULL, 0 );

		if( state == -1 )
		    state = ISSET_OPTION( r, Opt_useFifo ) ? 0 : 1;

		if( state == 1 && !ISSET_OPTION( r, Opt_useFifo ) )
		{
		    SET_OPTION( r, Opt_useFifo );
		    rxvt_init_fifo( r );
		}

		else if( state == 0 && ISSET_OPTION( r, Opt_useFifo ) )
		{
		    if( r->fifo_fd != -1 )
		    {
			close( r->fifo_fd );
			if( r->num_fds == r->fifo_fd + 1)
			    rxvt_adjust_fd_number( r );

			r->fifo_fd = -1;
			unlink( r->fifo_name );
		    }
		    UNSET_OPTION( r, Opt_useFifo );
		}
	    }
	    break;
#endif/*USE_FIFO*/

	case MacroFnPrintScreen:
	{
	    /*
	     * Arguments: [-{s,n,p}] command
	     *	s: Dump whole scroll back buffer.
	     *	p: Pretty print (i.e. with escape sequences for ANSII color)
	     *	n: No line continuation
	     * scroll back buffer. The argument if any is the command to use for
	     * the printer pipe.
	     */

	    char    *s		= (char*) astr;
	    int	    pretty	= 0,
		    scrollback	= 0,
		    linecont	= 1;


	    if( *s && *s == '-' )
	    {
		while( *(++s) && !isspace( *s ) )
		{
		    switch( *s )
		    {
			case 's': scrollback	= 1; break;
			case 'p': pretty	= 1; break;
			case 'n': linecont	= 0; break;
			default:
			    rxvt_msg (DBG_ERROR, DBG_MACROS,  "Bad option %c to macro %s",
				    *s, macroNames[action->type] );
			    retval = -1;
		    }
		}

		while( isspace(*s) ) s++;
	    }

	    rxvt_scr_printscreen( r, ATAB(r), scrollback, pretty, linecont,
		    *s ? s : NULL );

	    break;
	}

	case MacroFnSaveConfig:
	{
	    char    cfile[PATH_MAX] = "";

	    if (NOT_NULL(astr))
		STRNCPY( cfile, astr, PATH_MAX-1 );
	    else
	    {
		char*	home = getenv ("HOME");

		if (IS_NULL(home)) return -1; /* Failure */

		snprintf (cfile, PATH_MAX-1, "%s/%s", home,
			".mrxvtrc.save");
	    }
	    cfile[PATH_MAX-1] = (char) 0;   /* Null terminate */

	    retval = rxvt_save_options (r, cfile) ? 1 : -1;
	    break;
	}

	case MacroFnToggleMacros:
	    TOGGLE_OPTION(r, Opt2_disableMacros);
	    break;

	default:
	    assert( action->type < sizeof( macroNames ) / sizeof( char ** ) );

	    rxvt_msg (DBG_ERROR, DBG_MACROS,  "Support for macro type '%s' not compiled.",
		    macroNames[action->type]);
	    retval = -1;
    }

    return retval;
}