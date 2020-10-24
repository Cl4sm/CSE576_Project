void
rxvt_create_termwin( rxvt_t *r, int page, int profile,
	const char TAINTED *title )
{
    long	    vt_emask;


    assert( page < MAX_PAGES );

    rxvt_init_vts( r, page, profile );

    /*
     * Set the tab title
     */
    if (IS_NULL(title))
	title = DEFAULT_TAB_TITLE;
    PVTS(r, page)->tab_title = (char UNTAINTED *) STRNDUP( title, MAX_TAB_TXT );

#ifdef HAVE_PUTENV
    /* Set environment variable of tab title */
    sprintf (r->h->env_tabtitle, TABTITLEENV "%s", PVTS(r, page)->tab_title);
    putenv (r->h->env_tabtitle);
#endif

    PVTS(r, page)->tab_width = rxvt_tab_width (r, PVTS(r, page)->tab_title);

    /*
     * Now switch fg/bg colors before creating VT because this will use the
     * fg/bg colors
     */
    rxvt_set_vt_colors( r, page );

    /* create the terminal window */
    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "Create VT %d (%dx%d+%dx%d) fg=%06lx, bg=%06lx\n", page, r->h->window_vt_x, r->h->window_vt_y, VT_WIDTH(r), VT_HEIGHT(r), r->pixColors[Color_fg], r->pixColors[Color_bg]));

    PVTS(r, page)->vt = XCreateSimpleWindow (r->Xdisplay, r->TermWin.parent,
				r->h->window_vt_x, r->h->window_vt_y,
				VT_WIDTH(r), VT_HEIGHT(r),
				0,
				r->pixColors[Color_fg],
				r->pixColors[Color_bg]);
    assert (IS_WIN(PVTS(r, page)->vt));
#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft))
    {
	PVTS(r, page)->xftvt = XftDrawCreate (r->Xdisplay,
	    PVTS(r, page)->vt, XVISUAL, XCMAP);
	assert (NOT_NULL(PVTS(r, page)->xftvt));
    }
#endif


    /* define cursor for the terminal window */
    rxvt_pointer_unblank(r, page);

    /* define event mask fo the terminal window */
    vt_emask = (ExposureMask | ButtonPressMask | ButtonReleaseMask
	| PropertyChangeMask);
#ifdef POINTER_BLANK
    if (ISSET_OPTION(r, Opt_pointerBlank))
	vt_emask |= PointerMotionMask;
    else
#endif
	vt_emask |= (Button1MotionMask | Button3MotionMask);
    XSelectInput(r->Xdisplay, PVTS(r, page)->vt, vt_emask);

#ifdef TRANSPARENT
    /* Set transparent background */
    if (ISSET_OPTION(r, Opt_transparent))
    {
	XSetWindowBackgroundPixmap (r->Xdisplay, PVTS(r, page)->vt,
	    ParentRelative);
    }
#endif

    /*
     * Load the background image for terminal window when not transparent
     */
#ifdef BACKGROUND_IMAGE
# ifdef TRANSPARENT
    if( NOTSET_OPTION(r,  Opt_transparent) )
# endif
    {
	const char *pf = getProfileOption( r, profile, Rs_backgroundPixmap );
	if (NOT_NULL(pf))
	{
	    /* Load pixmap for each individual tab */
	    const char *p = pf;

	    if (NOT_NULL(p = STRCHR(p, ';')))
	    {
		p++;
		rxvt_scale_pixmap(r, page, p);
	    }
	    rxvt_load_bg_pixmap(r, page, pf);
	    /* rxvt_scr_touch(r, page, True); */
	}
    } /* if( NOTSET_OPTION(r,  Opt_transparent) ) */
#endif

    XMapWindow (r->Xdisplay, PVTS(r, page)->vt);
}