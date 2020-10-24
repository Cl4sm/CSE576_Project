const char**
rxvt_init_resources(rxvt_t* r, int argc, const char *const *argv)
{
    register int    i, r_argc;
    const char**    cmd_argv;
    const char**    r_argv;
    const char**    rs;


    /*
     * Look for -exec option.  Find => split and make cmd_argv[] of command args
     */
    for( r_argc = 0; r_argc < argc; r_argc++ )
	if( !STRCMP(argv[r_argc], "-e") || !STRCMP(argv[r_argc], "-exec") )
	    break;

    r_argv = (const char**) rxvt_malloc( sizeof(char*) * (r_argc + 1) );

    for( i = 0; i < r_argc; i++ )
	r_argv[i] = (const char*) argv[i];
    SET_NULL(r_argv[i]);

    if (r_argc == argc)
	SET_NULL(cmd_argv);
    else
    {
	cmd_argv = (const char **)rxvt_malloc(sizeof(char*) * (argc - r_argc));

	for (i = 0; i < argc - r_argc - 1; i++)
	    cmd_argv[i] = (const char *)argv[i + r_argc + 1];
	SET_NULL(cmd_argv[i]);
    }

    /* clear all resources */
    rs = r->h->rs;
    for (i = 0; i < NUM_RESOURCES;)
	SET_NULL(rs[i++]);

    rs[Rs_name] = rxvt_r_basename( argv[0] );

    /*
     * Open display, get options/resources and create the window
     */
    rxvt_get_options( r, r_argc, r_argv );
    rxvt_free( r_argv ); /* XXX memory leak? */

#ifdef LOCAL_X_IS_UNIX
    /*
     * 2008-04-29 gi1242: Force UNIX sockets for security (Gentoo Bug #219750)
     */
    if( IS_NULL( rs[Rs_display_name] ) )
	rs[Rs_display_name] = getenv( "DISPLAY" );

    if( rs[Rs_display_name] && rs[Rs_display_name][0] == ':' )
    {
	char *val;
	int l = 5 + STRLEN(rs[Rs_display_name]);
	if (l <= 0 || l > 1024) /* possible integer overflow */
	    l = 1024;

	val = rxvt_malloc(l);
	STRCPY( val, "unix");
	STRNCAT( val, rs[Rs_display_name], l-5);

	rs[Rs_display_name] = val;
    }
#endif /* LOCAL_X_IS_UNIX */

    rxvt_msg( DBG_INFO, DBG_INIT, "Open X display %s\n", rs[Rs_display_name] );
    r->Xdisplay = XOpenDisplay( rs[Rs_display_name] );
    if( IS_NULL(r->Xdisplay) )
    {
	rxvt_msg( DBG_ERROR, DBG_INIT, "Error opening display %s\n",
		rs[Rs_display_name] );
	exit( EXIT_FAILURE );
    }


#ifdef DEBUG_X
    /*
     * Makes life a lot simpler when handling X events, as they are not cached,
     * but processed immediately.
     */
    XSynchronize( r->Xdisplay, True );
#endif

    /*
     * Always set XErrorHandler to our own error handler because sometimes
     * errors are legal! Our error handler will abort when errors are not
     * allowed.
     */
    XSetErrorHandler( (XErrorHandler) rxvt_xerror_handler );

    /* Initialize all atoms after establishing connection to X */
    for (i = 0; i < NUM_XA; i++)
	r->h->xa[i] = XInternAtom( r->Xdisplay, xa_names[i], False );

    rxvt_extract_resources( r, r->Xdisplay, rs[Rs_name] );

    /*
     * set any defaults not already set
     */
    if( cmd_argv && cmd_argv[0] )
    {
	if( !rs[Rs_title] )
	    rs[Rs_title] = rxvt_r_basename( cmd_argv[0] );
	if( !rs[Rs_iconName] )
	    rs[Rs_iconName] = rs[Rs_title];
    }
    else
    {
	if( !rs[Rs_title] )
	    rs[Rs_title] = rs[Rs_name];
	if( !rs[Rs_iconName] )
	    rs[Rs_iconName] = rs[Rs_name];
    }

    if( rs[Rs_maxTabWidth] )
    {
	register int	tmp = atoi( rs[ Rs_maxTabWidth]);
	r->TermWin.maxTabWidth = ( tmp >=1 && tmp <= MAX_DISPLAY_TAB_TXT ) ?
					tmp : MAX_DISPLAY_TAB_TXT;
    }
    else
	/*
	 * If we're using Xft, then we will probably also use a PFont. So we
	 * should set this to the maximum possible.
	 */
	r->TermWin.maxTabWidth =
#ifdef XFT_SUPPORT
	    ISSET_OPTION(r, Opt_xft) ? MAX_DISPLAY_TAB_TXT :
#endif
	    DEFAULT_DISPLAY_TAB_TXT;


    if( rs[Rs_minVisibleTabs] )
    {
	register int	n = atoi( rs[Rs_minVisibleTabs] );
	r->TermWin.minVisibleTabs = (n >= 2 && n <= MAX_PAGES) ?
	    n : DEFAULT_MIN_VISIBLE_TABS;
    }
    else r->TermWin.minVisibleTabs = DEFAULT_MIN_VISIBLE_TABS;

#ifndef NO_FRILLS
    if (rs[Rs_int_bwidth])
    {
	register int	tmp = atoi( rs[Rs_int_bwidth] );
	r->TermWin.int_bwidth =( tmp >= 0 && tmp <= MAX_INTERNALBORDERWIDTH ) ?
					tmp : DEFAULT_INTERNALBORDERWIDTH;
    }

    if (rs[Rs_ext_bwidth])
    {
	register int	tmp = atoi( rs[Rs_ext_bwidth] );
	r->TermWin.ext_bwidth = (tmp >= 0 && tmp <= MAX_EXTERNALBORDERWIDTH) ?
					tmp : DEFAULT_EXTERNALBORDERWIDTH;
    }
#endif

#ifndef NO_LINESPACE
    if (rs[Rs_lineSpace])
    {
	register int	tmp = atoi( rs[Rs_lineSpace] );
	r->TermWin.lineSpace = (tmp >= 0 && tmp <= MAX_LINESPACE) ?
					tmp : DEFAULT_LINESPACE;
    }
#endif

#ifdef POINTER_BLANK
    if (rs[Rs_pointerBlankDelay])
    {
	register int	tmp = atoi( rs[Rs_pointerBlankDelay] );
	r->h->pointerBlankDelay = (tmp >= 0 && tmp <= MAX_BLANKDELAY) ?
					tmp : DEFAULT_BLANKDELAY;
    }
#endif

    /* Handle opacity of translucent window */
    if (rs[Rs_opacity])	
    {
	register int	tmp = atoi( rs[Rs_opacity] );
	r->TermWin.opacity = (tmp >= 0 && tmp <= 100) ? 100 - tmp : 0;

#ifdef TRANSPARENT
	if (
	      IS_ATOM(r->h->xa[XA_NET_WM_WINDOW_OPACITY]) &&
	      ISSET_OPTION(r, Opt_transparent)
	   )
	{
	    /* Override pseudo-transparent */
	    UNSET_OPTION(r, Opt_transparent);
	}
#endif
    }
    if (rs[Rs_opacityDegree])	
    {
	register int	tmp = atoi (rs[Rs_opacityDegree]);
	r->TermWin.opacity_degree = (tmp > 0 && tmp <= 100) ? tmp : 1;
    }

#ifdef TINTING_SUPPORT
    if (rs[Rs_shade])
    {
	register int	shade;
	shade = atoi( rs[Rs_shade] );
	if (shade < 0 || shade > 100)
	    shade = 100;
	r->TermWin.shade = 100 - shade;
    }
#endif

    rxvt_set_jumpscroll(r);

#ifdef TRANSPARENT
    if (rs[Rs_bgRefreshInterval])
    {
	register unsigned long interval = atol( rs[Rs_bgRefreshInterval] );

	if( interval > 1000 ) interval = 1000;
	r->h->bgRefreshInterval = interval * 1000L; /* convert to micro-sec */
    }
#endif

    if (rs[Rs_fade])
    {
	register int	fade;
	fade = atoi( rs[Rs_fade] );

	/*
	 * Fade levels of 0 will make the text completely black, so let's ignore
	 * it.
	 */
	if( fade <= 0 || fade > 100 )
	    fade = 100;
	r->TermWin.fade = 100 - fade;
    }
    /* else r->TermWin.fade is 0 */

#ifdef CURSOR_BLINK
    if (rs[Rs_cursorBlinkInterval])
    {
	register long	tmp = atol( rs[Rs_cursorBlinkInterval] );
	r->h->blinkInterval = (tmp >= MIN_BLINK_TIME && tmp <= MAX_BLINK_TIME) ? tmp : DEFAULT_BLINK_TIME;
    }
    /* convert msec to usec */
    r->h->blinkInterval *= 1000;
#endif

#ifdef PRINTPIPE
    if (!rs[Rs_print_pipe])
	rs[Rs_print_pipe] = PRINTPIPE;
#endif
    if (!rs[Rs_cutchars])
	rs[Rs_cutchars] = CUTCHARS;

#ifdef ACS_ASCII
    if( !rs[Rs_acs_chars] )
	rs[Rs_acs_chars] = ACS_CHARS;
    if( (i = STRLEN(rs[Rs_acs_chars])) < 0x20 )
    {
	char *val = rxvt_realloc( (void*) rs[Rs_acs_chars], 0x20 );
	for( ; i < 0x20; )
	    val[i++] = ' ';
	rs[Rs_acs_chars] = val;
    }
#endif

#ifndef NO_BACKSPACE_KEY
    if( !rs[Rs_backspace_key] )
# ifdef DEFAULT_BACKSPACE
	r->h->key_backspace = DEFAULT_BACKSPACE;
# else
	r->h->key_backspace = "DEC";	/* can toggle between \010 or \177 */
# endif
    else
    {
	char*	val = STRDUP(rs[Rs_backspace_key]);
	rxvt_str_trim( val );
	rxvt_str_escaped( val );
	r->h->key_backspace = val;
    }
#endif
#ifndef NO_DELETE_KEY
    if( !rs[Rs_delete_key] )
# ifdef DEFAULT_DELETE
	r->h->key_delete = DEFAULT_DELETE;
# else
	r->h->key_delete = "\033[3~";
# endif
    else
    {
	char *val = STRDUP( rs[Rs_delete_key] );
	rxvt_str_trim( val );
	rxvt_str_escaped( val );
	r->h->key_delete = val;
    }
#endif
    if( rs[Rs_answerbackstring] )
    {
	rxvt_str_trim( (char*) rs[Rs_answerbackstring] );
	rxvt_str_escaped( (char*) rs[Rs_answerbackstring] );
    }

    if( rs[Rs_selectstyle] )
    {
	if( STRNCASECMP( rs[Rs_selectstyle], "oldword", 7 ) == 0 )
	    r->selection_style = OLD_WORD_SELECT;
#ifndef NO_OLD_SELECTION
	else if( STRNCASECMP( rs[Rs_selectstyle], "old", 3 ) == 0 )
	    r->selection_style = OLD_SELECT;
#endif
    }


    /* Set default X11 fonts */
    rxvt_set_default_font_x11( r );
#ifdef XFT_SUPPORT
    if( rs[Rs_xftsz] )
    {
	int	sz = atoi( rs[Rs_xftsz] );
	r->TermWin.xftsize = (sz >= MIN_XFT_FONT_SIZE) ? sz : MIN_XFT_FONT_SIZE;
    }
    else /* default xft font size */
	r->TermWin.xftsize = DEFAULT_XFT_FONT_SIZE;

    if (rs[Rs_xftpsz])
    {
	int	sz = atoi (rs[Rs_xftpsz]);
	r->TermWin.xftpsize  =  (sz >= MIN_XFT_FONT_SIZE) ?
					sz : MIN_XFT_FONT_SIZE;
    }
    else /* default xft Pfont size */
	r->TermWin.xftpsize = DEFAULT_XFT_PFONT_SIZE;

# ifdef MULTICHAR_SET
    if (rs[Rs_xftmsz])
    {
	int	sz = (int) atof (rs[Rs_xftmsz]);
	r->TermWin.xftmsize = (sz >= MIN_XFT_FONT_SIZE) ?
					sz : MIN_XFT_FONT_SIZE;
    }
    else /* default xft font size */
	r->TermWin.xftmsize = DEFAULT_XFT_FONT_SIZE;
# endif	/* MULTICHAR_SET */

    /* Set default Freetype fonts */
    rxvt_set_default_font_xft (r);
#endif	/* XFT_SUPPORT */


#ifdef TEXT_SHADOW
    rxvt_init_shadow_mode (r, rs[Rs_textShadowMode]);
#endif

#ifdef XTERM_REVERSE_VIDEO
    /* this is how xterm implements reverseVideo */
    if (ISSET_OPTION(r, Opt_reverseVideo))
    {
	if (!rs[Rs_color + Color_fg])
	    rs[Rs_color + Color_fg] = def_colorName[Color_bg];
	if (!rs[Rs_color + Color_bg])
	    rs[Rs_color + Color_bg] = def_colorName[Color_fg];

	for (i = 0; i < MAX_PROFILES; i++)
	{
	    int	    vtfg = Rs_foreground + i;
	    int	    vtbg = Rs_background + i;

	    char*   fg = (char*) rs[vtfg];
	    char*   bg = (char*) rs[vtbg];

	    /* foreground color of i terminal */
	    if (ISSET_VTFG(r, i))
		rs[vtfg] = ISSET_VTBG(r, i) ? bg :
				def_colorName[Color_bg];
	    /* background color of i terminal */
	    if (ISSET_VTBG(r, i))
		rs[vtbg] = ISSET_VTFG(r, i) ? fg :
			    def_colorName[Color_fg];
	}
    }
#endif

    for (i = 0; i < NRS_COLORS; i++)
	if (!rs[Rs_color + i])
	    rs[Rs_color + i] = def_colorName[i];

#ifndef XTERM_REVERSE_VIDEO
    /* this is how we implement reverseVideo */
    if (ISSET_OPTION(r, Opt_reverseVideo))
    {
	if (!rs[Rs_color + Color_fg])
	    rs[Rs_color + Color_fg] = def_colorName[Color_fg];
	if (!rs[Rs_color + Color_bg])
	    rs[Rs_color + Color_bg] = def_colorName[Color_bg];

	SWAP_IT(rs[Rs_color + Color_fg], rs[Rs_color + Color_bg], const char *);

	for (i = 0; i < MAX_PROFILES; i++)
	{
	    int	    vtfg = Rs_foreground + i;
	    int	    vtbg = Rs_background + i;
	    if (!rs[vtfg])
		rs[vtfg] = def_colorName[Color_fg];
	    if (!rs[vtbg])
		rs[vtbg] = def_colorName[Color_bg];

	    SWAP_IT(rs[vtfg], rs[vtbg], const char*);
	}
    }
#endif

    /* convenient aliases for setting fg/bg to colors */
    rxvt_color_aliases(r, Color_fg);
    rxvt_color_aliases(r, Color_bg);
#ifndef NO_CURSORCOLOR
    rxvt_color_aliases(r, Color_cursor);
    rxvt_color_aliases(r, Color_cursor2);
#endif		    /* NO_CURSORCOLOR */
    rxvt_color_aliases(r, Color_pointer);
    rxvt_color_aliases(r, Color_border);
#ifndef NO_BOLD_UNDERLINE_REVERSE
    rxvt_color_aliases(r, Color_BD);
    rxvt_color_aliases(r, Color_UL);
    rxvt_color_aliases(r, Color_RV);
#endif		    /* ! NO_BOLD_UNDERLINE_REVERSE */

    /*
     * On startup, use autohideTabbar to override hideTabbar. Thus on startup,
     * using autohideTabbar will only display the tabbar if there are multiple
     * tabs. The user can hide / show the tabbar using a macro at will.
     */
    if(ISSET_OPTION(r, Opt2_autohideTabbar))
	SET_OPTION(r, Opt2_hideTabbar);

    /* Cleanup the macro list */
    rxvt_cleanup_macros( r );


    /*
     * Profile settings.
     */
    for( i=0; i < MAX_PROFILES; i++ )
    {
	/* Set saveLines */
	if( r->h->rs[Rs_saveLines + i] )
	{
	    int tmp = atoi( r->h->rs[Rs_saveLines + i] );

	    r->profile[i].saveLines = ( tmp >= 0 && tmp <= MAX_SAVELINES ) ?
		    tmp : DEFAULT_SAVELINES;
	}
	else
	    r->profile[i].saveLines = ( i > 0 ) ? r->profile[0].saveLines :
							    DEFAULT_SAVELINES;

	/* Set holdOption */
	if( r->h->rs[Rs_holdExit + i] )
	{
	    const char *s = r->h->rs[Rs_holdExit + i];

	    /* Backward compatibility hack */
	    if(
		 !STRCASECMP( s, "true" )	    ||
		 !STRCASECMP( s, "yes" )	    ||
		 !STRCASECMP( s, "on" )
	      )
		r->profile[i].holdOption = HOLD_ALWAYSBIT;
	    else
		r->profile[i].holdOption = strtoul( s, NULL, 0 );
	}
	else
	    r->profile[i].holdOption = (i > 0) ? r->profile[0].holdOption :
					    (HOLD_STATUSBIT|HOLD_NORMALBIT);
    } /* for(i) */

    if( !r->h->rs[Rs_holdExitTtl] )
	r->h->rs[Rs_holdExitTtl] = "(Done) %t";

    if( !r->h->rs[Rs_holdExitTxt] )
	r->h->rs[Rs_holdExitTxt] = "\n\n\r\e[31m"
				   "Process exited %N with status %S. "
				   "Press any key to close tab.\e[0m";

#ifdef OS_LINUX
    if( !r->h->rs[Rs_cwd] )
	r->h->rs[Rs_cwd] = ".";
#endif

#ifndef NO_BEEP
    if( r->h->rs[Rs_vBellDuration] )
	r->TermWin.vBellDuration =
	    1000000ul * strtoul( r->h->rs[Rs_vBellDuration], NULL, 0 );
    else
	r->TermWin.vBellDuration = 0;
#endif


    return cmd_argv;
}