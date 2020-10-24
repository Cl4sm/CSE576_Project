void
rxvt_init_vts( rxvt_t *r, int page, int profile )
{
#ifdef TTY_GID_SUPPORT
    struct group*   gr = getgrnam( "tty" );
#endif
    register int    i;


    assert( page < MAX_PAGES );

    /* look for an unused term_t structure */
    for( i = 0; i < MAX_PAGES; i ++ )
	if( -1 == r->vterm[i].vts_idx )
	    break;
    assert( i != MAX_PAGES );
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "Find vterm[%d] for pointer vts[%d]\n", i, page));

    /* clear the term_t structure */
    r->vts[page] = &(r->vterm[i]);
    MEMSET( r->vts[page], 0, sizeof( r->vterm[0] ) );

    /* set vts_idx for the vterm */
    PVTS(r, page)->vts_idx = i;

    /* Set the profile number */
    PVTS(r, page)->profileNum	= profile;

    /* Save the "static" number of this tab */
    PVTS(r, page)->globalTabNum	= r->ntabs++;

#ifdef TTY_GID_SUPPORT
    /* change group ownership of tty to "tty" */
    if (gr)
    {
	PVTS(r, page)->ttymode = S_IRUSR | S_IWUSR | S_IWGRP;
    }
    else
#endif	    /* TTY_GID_SUPPORT */
    {
	PVTS(r, page)->ttymode = S_IRUSR | S_IWUSR | S_IWGRP | S_IWOTH;
    }

    /* Initialize term_t (vts) structure */
    PVTS( r, page )->saveLines = r->profile[profile].saveLines;

    /* will be set in rxvt_create_termwin */
    UNSET_WIN(PVTS(r, page)->vt);

#ifdef XFT_SUPPORT
    SET_NULL(PVTS(r, page)->xftvt);
#endif
    SET_NULL(PVTS(r, page)->tab_title);

    /*
     * Set the tab title format, and window title format. getProfileOption
     * returns a static string, so duplicate it here
     */
    {
	const char *stf = getProfileOption( r, profile, Rs_titleFormat );
	PVTS(r, page)->title_format = NOT_NULL(stf) ? STRDUP(stf) : NULL;
    }

    {
	const char *wtf = getProfileOption( r, profile, Rs_winTitleFormat );
	PVTS(r, page)->winTitleFormat = NOT_NULL(wtf) ? STRDUP(wtf) : NULL;
    }

#ifdef BACKGROUND_IMAGE
    UNSET_PIXMAP(PVTS(r, page)->pixmap);
    UNSET_PIXMAP(PVTS(r, page)->bg.pixmap);
    PVTS(r, page)->bg.x = PVTS(r, page)->bg.y = 50;
#endif
    PVTS(r, page)->cmd_pid = -1;
    PVTS(r, page)->cmd_fd = PVTS(r, page)->tty_fd = -1;
#ifdef UTMP_SUPPORT
    PVTS(r, page)->next_utmp_action = SAVE;
#endif
#ifndef NO_SETOWNER_TTYDEV
    PVTS(r, page)->next_tty_action = SAVE;
#endif

    PVTS(r, page)->holdOption = r->profile[profile].holdOption;

    PVTS(r, page)->status = 0;
    PVTS(r, page)->hold = 0;	    /* clear hold flag */
    PVTS(r, page)->dead = 0;	    /* clear dead flag */
    PVTS(r, page)->highlight = 0;   /* clear highlight flag */

    /* Get term_env type */
    PVTS(r, page)->termenv = rxvt_get_termenv (
	r->h->rs[Rs_term_name] ? r->h->rs[Rs_term_name] : TERMENV);

    /* Initialize PrivateModes and SavedModes */
    PVTS(r, page)->PrivateModes = PVTS(r, page)->SavedModes =
	PrivMode_Default;
    if (ISSET_OPTION(r, Opt_scrollTtyOutputInhibit))
	SET_PMODE(r, page, PrivMode_TtyOutputInh);
    if (ISSET_OPTION(r, Opt_scrollTtyKeypress))
	SET_PMODE(r, page, PrivMode_Keypress);
    if( r->h->skip_pages > 1 /* jump scroll is unset */ )
	SET_PMODE(r, page, PrivMode_smoothScroll);
#ifndef NO_BACKSPACE_KEY
    if (STRCMP(r->h->key_backspace, "DEC") == 0)
	SET_PMODE(r, page, PrivMode_HaveBackSpace);
#endif
#ifdef HAVE_SCROLLBARS
    if (rxvt_scrollbar_visible(r))
    {
	SET_PMODE(r, page, PrivMode_scrollBar);
	SET_SMODE(r, page, PrivMode_scrollBar);
    }
#endif
#ifdef HAVE_MENUBAR
    if (rxvt_menubar_visible(r))
    {
	SET_PMODE(r, page, PrivMode_menuBar);
	SET_SMODE(r, page, PrivMode_menuBar);
    }
#endif

    /* Now set VT fg/bg color */
    PVTS(r, page)->p_fg = VTFG(r, profile);
    PVTS(r, page)->p_bg = VTBG(r, profile);

    if( r->TermWin.fade )
    {
	PVTS(r, page)->p_fgfade = VTFG_FADE(r, profile);
	PVTS(r, page)->p_bgfade = VTBG_FADE(r, profile);
    }

#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
    {
	PVTS(r, page)->p_xftfg = VTXFTFG(r, profile);
	PVTS(r, page)->p_xftbg = VTXFTBG(r, profile);

	if( r->TermWin.fade )
	{
	    PVTS(r, page)->p_xftfgfade = VTXFTFG_FADE(r, profile);
	    PVTS(r, page)->p_xftbgfade = VTXFTBG_FADE(r, profile);
	}
    }
#endif

    /* Initialize input buffer */
    PVTS(r, page)->cmdbuf_ptr	= PVTS(r, page)->cmdbuf_endp
				= PVTS(r, page)->cmdbuf_base;
    
    /* Initialize write out buffer */
    SET_NULL(PVTS(r, page)->v_buffer);
    SET_NULL(PVTS(r, page)->v_bufstr);
    SET_NULL(PVTS(r, page)->v_bufptr);
    SET_NULL(PVTS(r, page)->v_bufend);

    /* Set screen structure initialization flag */
    PVTS(r, page)->init_screen = 0;

    /* Request a refresh */
    PVTS(r, page)->want_refresh = 1;
}