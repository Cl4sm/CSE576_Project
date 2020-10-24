void
rxvt_create_show_windows( rxvt_t *r, int argc, const char *const *argv )
{
    XClassHint		    class_hint;
    XWMHints		    wm_hint;
    XTextProperty	    win_prop;
    XTextProperty	    icon_prop;
    XGCValues		    gcvalue;
    unsigned long	    gcmask;
#ifndef NO_FRILLS
    CARD32		    pid = (CARD32) getpid ();
#endif
#ifdef TRANSPARENT
    register int	    i;
#endif
#ifdef POINTER_BLANK
    static const XColor	    blackcolour = { 0, 0, 0, 0, 0, 0 };
#endif

    Window		    parent;	/* WinID to use for parent window */

#ifdef PREFER_24BIT
    XSetWindowAttributes    attributes;
    XWindowAttributes	    gattr;


    XCMAP = DefaultColormap(r->Xdisplay, XSCREEN);
    XVISUAL = DefaultVisual(r->Xdisplay, XSCREEN);

    if (ISSET_OPTION(r, Opt_transparent))
    {
	XGetWindowAttributes(r->Xdisplay,
	    RootWindow(r->Xdisplay, XSCREEN), &gattr);
	XDEPTH = gattr.depth;
    }
    else
    {
	XDEPTH = DefaultDepth(r->Xdisplay, XSCREEN);
	/*
	 * If depth is not 24, look for a 24bit visual.
	 */
	if (XDEPTH != 24)
	{
	    XVisualInfo	 vinfo;

	    if (XMatchVisualInfo(r->Xdisplay, XSCREEN, 24, TrueColor, &vinfo))
	    {
		XDEPTH = 24;
		XVISUAL = vinfo.visual;
		XCMAP = XCreateColormap(r->Xdisplay,
			    RootWindow(r->Xdisplay, XSCREEN),
			    XVISUAL, AllocNone);
	    }
	}
    }
#endif


    /* grab colors before netscape does */
    rxvt_init_colors (r);

    /*
     * Initialize fonts.
     * . Always load X11 fonts since pointer_blank uses it
     * . Load XFT font after X11 fonts. If succeeds, XFT font will
     * update font width/height and be used by default
     *
     * 03/09/2006 gi1242: TODO Don't load the X11 font unless absolutely
     * necessary. It will speed up startup (minimaly), and reduce resource
     * usage (minimaly).
     */
#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft))
    {
	if (!rxvt_init_font_xft (r))
	{
	    rxvt_msg (DBG_INFO, DBG_INIT,
		    "Failed to load FreeType font, fallback to X11 font\n");
	    /* disable xft */
	    UNSET_OPTION(r, Opt_xft);
	}
	else
	    xftInitACS (r->Xdisplay, XROOT, XDEPTH);
    }
#endif
    /* init fallback X11 font */
    rxvt_init_font_x11( r );


    /*
     * must initialize scrollbar before initialize window size and
     * create windows.
     */
#ifdef HAVE_SCROLLBARS
    rxvt_scrollbar_init (r);
#endif
    rxvt_init_win_size (r);

    /*
     * Use window specified by -into option as the parent window.
     */
    if( r->h->rs[Rs_container_window] )
    {
	XWindowAttributes   attrs;

	r->h->allowedxerror = 1;    /* Enable Xerror reporting */
	r->h->xerror_return = Success;

        parent = strtoul( r->h->rs[Rs_container_window], NULL, 0 );

	XGetWindowAttributes( r->Xdisplay, parent, &attrs );

	/* Check if we have valid attributes */
	if( r->h->xerror_return != Success || attrs.class == InputOnly )
	{
	    rxvt_msg (DBG_ERROR, DBG_INIT,  "Unable to embed into Win 0x%lx", parent );
	    parent = XROOT;
	}

	r->h->allowedxerror = 0;    /* Disable Xerror reporting */
    }
    else
        parent = XROOT;
    
    /*
     * parent window - reverse video so we can see placement errors sub-window
     * placement & size in rxvt_resize_subwindows()
     */

#ifdef PREFER_24BIT
    attributes.background_pixel = r->pixColorsFocus[Color_bg];
    attributes.border_pixel = r->pixColorsFocus[Color_border];
    attributes.colormap = XCMAP;
    r->TermWin.parent = XCreateWindow(r->Xdisplay, parent,
		    r->szHint.x, r->szHint.y,
		    r->szHint.width, r->szHint.height,
		    r->TermWin.ext_bwidth,
		    XDEPTH, InputOutput,
		    XVISUAL,
		    CWBackPixel | CWBorderPixel
		    | CWColormap, &attributes);
#else
    r->TermWin.parent = XCreateSimpleWindow(r->Xdisplay, parent,
			r->szHint.x, r->szHint.y,
			r->szHint.width,
			r->szHint.height,
			r->TermWin.ext_bwidth,
			r->pixColorsFocus[Color_border],
			r->pixColorsFocus[Color_bg]);
#endif

#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft))
    {
	/* create XFT draw, test only */
	XftDraw*	xftdraw = XftDrawCreate( r->Xdisplay,
						 r->TermWin.parent, XVISUAL,
						 XCMAP);
	if (xftdraw)
	{
	    XftDrawDestroy (xftdraw);
	    /* some cleanup work if successfully create xft window */
# ifdef POINTER_BLANK
	    /*
	     * 2006-01-21 gi1242: I'm not sure why pointer blank is disabled
	     * with xft. It works fine for me, so I re-enabled it.
	     */
#  if 0
	    /* disable pointer blank */
	    UNSET_OPTION(r, Opt_pointerBlank);
#  endif
# endif
	}
    }
#endif


# ifdef HAVE_X11_SM_SMLIB_H
    if (ISSET_OPTION(r, Opt2_enableSessionMgt))
	rxvt_session_init (r);
# endif


    /*
     * Now set window properties, like title, icon name and hints
     */
    /* window title name */
    win_prop.value = (unsigned char*) r->h->rs[Rs_title];
    win_prop.nitems = STRLEN (win_prop.value);
    win_prop.encoding = XA_STRING;
    win_prop.format = 8; 
    /* icon name */
    icon_prop.value = (unsigned char*) r->h->rs[Rs_iconName];
    icon_prop.nitems = STRLEN (icon_prop.value);
    icon_prop.encoding = XA_STRING;
    icon_prop.format = 8; 
    /* window manager hints */
    wm_hint.flags = (InputHint | StateHint | WindowGroupHint);
    wm_hint.input = True;
    wm_hint.initial_state = ISSET_OPTION(r, Opt_iconic) ? IconicState
			    : NormalState;
    wm_hint.window_group = r->TermWin.parent;
    /* window icon hint */
#ifdef HAVE_LIBXPM
    if( r->h->rs[Rs_appIcon] )
    {
	Pixmap appIcon, appIconMask;

        XpmReadFileToPixmap( r->Xdisplay, r->TermWin.parent,
		(char*) r->h->rs[Rs_appIcon], &appIcon, &appIconMask, 0);

	if( appIcon != None &&  appIconMask != None ) {
	    wm_hint.icon_pixmap = appIcon;
	    wm_hint.icon_mask = appIconMask;
	    wm_hint.flags |= IconPixmapHint | IconMaskHint;
	}
    }
#endif /* HAVE_LIBXPM */
    /* class hints */
    class_hint.res_name = (char*) r->h->rs[Rs_name];
    class_hint.res_class = (char*) APL_CLASS;
    XSetWMProperties (r->Xdisplay, r->TermWin.parent,
	&win_prop, &icon_prop, (char**)argv, argc,
	&r->szHint, &wm_hint, &class_hint);

#if 0 /* If the pixmap's are free'ed, then the WM will not display them. */
    if( wm_hint.flags & IconPixmapHint )
    {
	XFreePixmap( r->Xdisplay, wm_hint.icon_pixmap );
	XFreePixmap( r->Xdisplay, wm_hint.icon_mask );
    }
#endif

    /* set terminal title */
    rxvt_set_term_title (r, win_prop.value);
    /* set icon title */
    rxvt_set_icon_name (r, icon_prop.value);
    /* command line */
    XSetCommand (r->Xdisplay, r->TermWin.parent, (char**) argv, argc);

    /* override redirect */
    if (ISSET_OPTION(r, Opt2_overrideRedirect))
    {
	XSetWindowAttributes	attrib;
	attrib.override_redirect = True;
	XChangeWindowAttributes(r->Xdisplay, r->TermWin.parent,
	    CWOverrideRedirect, &attrib);
    }

#ifndef NO_FRILLS
    XChangeProperty (r->Xdisplay, r->TermWin.parent,
	r->h->xa[XA_NET_WM_PID], XA_CARDINAL, 32,
	PropModeReplace, (unsigned char*) &pid, 1);
#endif

    if (ISSET_OPTION(r, Opt2_borderLess))
    {
	rxvt_set_borderless (r);
    }
    if (r->h->rs[Rs_desktop])
    {
	CARD32	desktop = (CARD32) atoi (r->h->rs[Rs_desktop]);
	rxvt_set_desktop (r, desktop);
    }

    /*
     * set WM_CLIENT_LEADER property so that session management proxy can handle
     * us even session management is not enabled.
     */
    if (IS_ATOM(r->h->xa[XA_WM_CLIENT_LEADER]))
	XChangeProperty( r->Xdisplay, r->TermWin.parent,
	    r->h->xa[XA_WM_CLIENT_LEADER], XA_WINDOW, 32,
	    PropModeReplace, (unsigned char*) &(r->TermWin.parent), 1L );

# ifdef HAVE_X11_SM_SMLIB_H
    if (NOT_NULL(r->TermWin.sm_conn) &&
	NOT_NULL(r->TermWin.sm_client_id) &&
	STRCMP (r->TermWin.sm_client_id, "")
       )
    {
	if (IS_ATOM(r->h->xa[XA_SM_CLIENT_ID]))
	    XChangeProperty(r->Xdisplay, r->TermWin.parent,
		r->h->xa[XA_SM_CLIENT_ID], XA_STRING, 8,
		PropModeReplace,
		(unsigned char*) r->TermWin.sm_client_id, 
		STRLEN(r->TermWin.sm_client_id));
    }
# endif	/* HAVE_X11_SM_SMLIB_H */


#ifdef TRANSPARENT
    r->TermWin.parenttree[0] = r->TermWin.parent;
    for (i = 1; i < PARENT_NUMBER; i ++)
	UNSET_WIN(r->TermWin.parenttree[i]);

    /*
     * XXX 2006-01-02 gi1242: This is inefficient. If window is pseudo
     * transparent, then the background pixmap will be reset later to something
     * else.
     */
#if 0
    if (ISSET_OPTION(r, Opt_transparent))
    {
	XSetWindowBackgroundPixmap (r->Xdisplay, r->TermWin.parent,
	    ParentRelative);
    }
#endif
#endif	/* TRANSPARENT */


    XSelectInput(r->Xdisplay, r->TermWin.parent,
	    (KeyPressMask
#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
	    | KeyReleaseMask
#endif
	    | FocusChangeMask
#ifdef MONITOR_ENTER_LEAVE
	    | EnterWindowMask | LeaveWindowMask
#endif
	    | VisibilityChangeMask
	    | StructureNotifyMask));


    /*
    ** vt cursor: Black-on-White is standard, but this is more
    ** popular
    */
    r->term_pointer = XCreateFontCursor(r->Xdisplay, XC_xterm);
    /* scrollbar/menubar/tabbar window pointer */
    r->h->bar_pointer = XCreateFontCursor(r->Xdisplay, XC_left_ptr);

#ifdef POINTER_BLANK
    if (NOTSET_OPTION(r, Opt_pointerBlank))
	UNSET_CURSOR(r->h->blank_pointer);
    else
	r->h->blank_pointer = XCreateGlyphCursor(r->Xdisplay,
	    r->TermWin.font->fid, r->TermWin.font->fid, ' ', ' ',
	    (XColor*) &blackcolour, (XColor*) &blackcolour);
#endif


    /* graphics context for the vt window */
#ifdef XFT_SUPPORT
    if (NOTSET_OPTION(r, Opt_xft))
#endif
    gcvalue.font = r->TermWin.font->fid;
    gcvalue.foreground = r->pixColorsFocus[Color_fg];
    gcvalue.background = r->pixColorsFocus[Color_bg];
    gcvalue.graphics_exposures = 1;
    gcmask = GCForeground | GCBackground | GCGraphicsExposures;
#ifdef XFT_SUPPORT
    if (NOTSET_OPTION(r, Opt_xft))
#endif
    gcmask |= GCFont;
    r->TermWin.gc = XCreateGC(r->Xdisplay, r->TermWin.parent,
		    gcmask, &gcvalue);

#ifdef HAVE_SCROLLBARS
    rxvt_scrollbar_create (r);
    if (ISSET_OPTION(r, Opt_scrollBar))
    {
	rxvt_scrollbar_show (r);
    }
#endif
#ifdef HAVE_MENUBAR
    if (r->h->rs[Rs_menu] && STRCASECMP( r->h->rs[Rs_menu], "none"))
    {
	/*
	 * Only load menubar if arg of -menu option is not none
	 */
	rxvt_menubar_load_file (r, (unsigned char*) r->h->rs[Rs_menu]);
    }
    else rxvt_menubar_load_file( r, (unsigned char*) "default.menu");

    rxvt_menubar_create (r);
    if (ISSET_OPTION(r, Opt_showMenu))
	rxvt_menubar_show (r);

    /*
     * 2006-05-28 gi1242: If popup menu 1 is not defined, set it to an empty
     * menu (so that the tab list will be popped up on control clicks and right
     * clicks on the tabbar).
     */
    if (IS_NULL(r->h->popupMenu[0]))
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "Setting popup menu 1 to a tab list\n"));
	r->h->popupMenu[0] = (menu_t *) rxvt_calloc( 1, sizeof(menu_t) );

	r->h->popupMenu[0]->len	    = sizeof( "Switch to tab" );
	r->h->popupMenu[0]->name    = (unsigned char*) STRDUP ("Switch to tab");
    }
# endif

    rxvt_tabbar_create (r);
    if (NOTSET_OPTION(r, Opt2_hideTabbar))
	rxvt_tabbar_show (r);

    XMapWindow (r->Xdisplay, r->TermWin.parent);

    /*
     * We have to wait till our window is mapped before we can set the maximized
     * or fullscreen options.
     */
    if( ISSET_OPTION(r, Opt2_maximized))
	ewmh_message( r->Xdisplay, XROOT, r->TermWin.parent,
	    XInternAtom( r->Xdisplay, "_NET_WM_STATE", True),
	    _NET_WM_STATE_ADD,
	    XInternAtom( r->Xdisplay, "_NET_WM_STATE_MAXIMIZED_HORZ", True),
	    XInternAtom( r->Xdisplay, "_NET_WM_STATE_MAXIMIZED_VERT", True),
	    0, 0);
    else if (ISSET_OPTION (r, Opt2_fullscreen))
	ewmh_message( r->Xdisplay, XROOT, r->TermWin.parent,
	    XInternAtom( r->Xdisplay, "_NET_WM_STATE", True),
	    _NET_WM_STATE_ADD,
	    XInternAtom( r->Xdisplay, "_NET_WM_STATE_FULLSCREEN", True),
	    0, 0, 0);
}