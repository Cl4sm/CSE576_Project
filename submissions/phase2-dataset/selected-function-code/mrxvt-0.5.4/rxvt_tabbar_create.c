void
rxvt_tabbar_create (rxvt_t* r)
{
    XColor	    color, bgcolor;
    XGCValues	    gcvalue;
    unsigned long   gcmask;
    register int    i;
    int		    sx, sy;
#ifdef HAVE_LIBXPM
    XpmAttributes   xpm_attr;
    /*
     * Make sure symbol `background' exists in all .xpm files! This elimate the
     * background color so that the buttons look transparent.
     */
    XpmColorSymbol  xpm_color_sym = {"background", NULL, 0};
#endif


    init_tabbar (r);
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"Creating tabbar\n"));


    /* initialize the colors */
    if (XDEPTH <= 2)
    {
	r->tabBar.fg = r->pixColorsFocus[Color_fg];
	r->tabBar.bg = r->pixColorsFocus[Color_bg];
	r->tabBar.ifg = r->pixColorsFocus[Color_fg];
	r->tabBar.ibg = r->pixColorsFocus[Color_bg];
	r->tabBar.frame = r->pixColorsFocus[Color_bg];
	r->tabBar.delimit = r->pixColorsFocus[Color_fg];
    }
    else 
    {
	/* create the foreground color */
	if(
	     r->h->rs[Rs_tabfg] && 
	     rxvt_parse_alloc_color (r, &color, r->h->rs[Rs_tabfg])
	  )
	{
	    r->tabBar.fg = color.pixel;
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		rxvt_alloc_xft_color( r, &color, &r->tabBar.xftfg );
#endif
	}

	else
	{
	    r->tabBar.fg = r->pixColorsFocus[Color_Black];
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		r->tabBar.xftfg = r->xftColorsFocus[Color_Black];
#endif
	}

	/*
	 * create the background color
	 */
	if(
	     r->h->rs[Rs_tabbg]	&&
	     rxvt_parse_alloc_color (r, &color, r->h->rs[Rs_tabbg])
	  )
	{
	    r->tabBar.bg = color.pixel;
	}

	else
	{
	    color.red	= 0xd300;
	    color.green = 0xd300;
	    color.blue	= 0xdd00;
	    if( rxvt_alloc_color (r, &color, "Active_Tab") )
		r->tabBar.bg = color.pixel;
	    else
		r->tabBar.bg = VTBG(r,0);
	}

	/* create the tab frame color */
	r->tabBar.frame = r->pixColorsFocus[Color_fg];

	/* create the inactive tab foreground color */
	if(
	     r->h->rs[Rs_itabfg]
	     && rxvt_parse_alloc_color (r, &color, r->h->rs[Rs_itabfg])
	  )
	{
	    r->tabBar.ifg = color.pixel;
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		rxvt_alloc_xft_color( r, &color, &r->tabBar.xftifg );
#endif
	}
	else
	{
	    r->tabBar.ifg = r->pixColorsFocus[Color_Black];
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		r->tabBar.xftifg = r->xftColorsFocus[Color_Black];
#endif
	}

	/* create the inactive tab background color */
	if(
	     r->h->rs[Rs_itabbg]
	     && rxvt_parse_alloc_color( r, &color, r->h->rs[Rs_itabbg] )
	  )
	    r->tabBar.ibg = color.pixel;

	else
	{
	    color.red	= 0xa100;
	    color.green = 0xa100;
	    color.blue	= 0xac00;
	    if( rxvt_alloc_color( r, &color, "Inactive_Tab_Bg" ) )
		r->tabBar.ibg = color.pixel;
	    else
		r->tabBar.ibg = VTBG(r,0);
	}

	/* create the delimit color (average of 3*fg & bg) */
	color.pixel	= r->pixColorsFocus[Color_fg];
	XQueryColor( r->Xdisplay, XCMAP, &color );

	bgcolor.pixel	= r->pixColorsFocus[Color_bg];
	XQueryColor( r->Xdisplay, XCMAP, &bgcolor );

	color.red   = ( bgcolor.red	+ 3 * color.red	    ) / 4;
	color.green = ( bgcolor.green	+ 3 * color.green   ) / 4;
	color.blue  = ( bgcolor.blue	+ 3 * color.blue    ) / 4;

	if( rxvt_alloc_color( r, &color, "Tab_Delimit" ) )
	    r->tabBar.delimit = color.pixel;
	else
	    r->tabBar.delimit = VTFG(r,0);

	rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "Delimit color: %hx, %hx, %hx (#%lx)\n", color.red, color.green, color.blue, r->tabBar.delimit));
    }


    sx = 0;
    sy = 0;
#ifdef HAVE_MENUBAR
    sy += rxvt_menubar_height (r);
#endif
    if (ISSET_OPTION(r, Opt2_bottomTabbar))
	sy += VT_HEIGHT(r);
    /*
     * create the window of the tabbar. Use ifg and ibg for the background of
     * the tabBar so that the active tab stands out better.
     */
    r->tabBar.win = XCreateSimpleWindow( r->Xdisplay, r->TermWin.parent,
			sx, sy, TWIN_WIDTH(r), rxvt_tabbar_rheight( r ),
			0, r->tabBar.ifg, r->tabBar.ibg );
    assert(IS_WIN(r->tabBar.win));

#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft))
    {
	r->tabBar.xftwin = XftDrawCreate (r->Xdisplay, r->tabBar.win,
				XVISUAL, XCMAP);
    }
#endif

#ifdef DEBUG_X
    rxvt_set_win_title (r, r->tabBar.win, "tabbar");
#endif


#ifdef BACKGROUND_IMAGE
    r->tabBar.hasPixmap = False;    /* initialize it to None */
    if (
#ifdef TRANSPARENT
	    /* Transparency overrides background */
	    !(
		ISSET_OPTION(r, Opt_transparent)
		&& ISSET_OPTION(r, Opt_transparent_tabbar)
	     )
	    &&
#endif
	    r->h->rs[Rs_tabbarPixmap]
       )
    {
	long	w = 0, h = 0;
	Pixmap	pmap;

	pmap = rxvt_load_pixmap (r, r->h->rs[Rs_tabbarPixmap], &w, &h);
	if (IS_PIXMAP(pmap))
	{
	    XSetWindowBackgroundPixmap (r->Xdisplay, r->tabBar.win, pmap);
	    XFreePixmap( r->Xdisplay, pmap);

	    r->tabBar.hasPixmap = True;
	}
	else r->tabBar.hasPixmap = False;
    }
#endif

#ifdef TRANSPARENT
    if (
	    ISSET_OPTION(r, Opt_transparent)
	    && ISSET_OPTION(r, Opt_transparent_tabbar)
       )
	XSetWindowBackgroundPixmap( r->Xdisplay, r->tabBar.win, ParentRelative);
#endif


    /* create the GC for the tab window */
    gcvalue.foreground	= r->tabBar.fg;
    gcvalue.line_width	= 0;
    gcvalue.line_style	= LineSolid;
    gcvalue.cap_style	= CapButt;
    gcvalue.join_style	= JoinMiter;
    gcvalue.arc_mode	= ArcChord;	/* For coloring ATAB */
    gcvalue.fill_style	= FillSolid;	/* Probably default ... */

    gcmask = GCForeground | GCLineWidth
		| GCLineStyle | GCCapStyle | GCJoinStyle
		| GCArcMode | GCFillStyle;

#ifdef TRANSPARENT
    /* set background color when there's no transparent */
    if (!(( r->h->am_transparent || r->h->am_pixmap_trans) &&
	ISSET_OPTION(r, Opt_transparent_tabbar)))
#endif
#ifdef BACKGROUND_IMAGE
	/* set background color when there's no bg image */
	if ( ! r->tabBar.hasPixmap )
#endif
	{
	    gcvalue.background = r->tabBar.bg;
	    gcmask |= GCBackground;
	}

    r->tabBar.gc = XCreateGC (r->Xdisplay, r->tabBar.win,
	gcmask, &gcvalue);
    assert (IS_GC(r->tabBar.gc));


    XDefineCursor (r->Xdisplay, r->tabBar.win, r->h->bar_pointer);
    XSelectInput (r->Xdisplay, r->tabBar.win,
	    ExposureMask | ButtonPressMask | ButtonReleaseMask
#ifdef HAVE_MENUBAR
		| Button3MotionMask
#endif
	);

#ifdef XFT_SUPPORT
    if (NOTSET_OPTION(r, Opt_xft))
#endif
    XSetFont (r->Xdisplay, r->tabBar.gc, r->TermWin.font->fid);


#ifdef HAVE_LIBXPM
    xpm_color_sym.pixel = r->tabBar.bg;
    xpm_attr.colorsymbols = &xpm_color_sym;
    xpm_attr.numsymbols = 1;
    xpm_attr.visual = XVISUAL;
    xpm_attr.colormap = XCMAP;
    xpm_attr.depth = XDEPTH;
    xpm_attr.closeness = 65535;
    xpm_attr.valuemask = XpmVisual | XpmColormap | XpmDepth |
	XpmCloseness | XpmReturnPixels | XpmColorSymbols;
#endif

    /* now, create the buttons */
    for (i = 0; i < NB_XPM; i++)
    {
#ifdef HAVE_LIBXPM
	XpmCreatePixmapFromData (r->Xdisplay, r->tabBar.win,
	    xpm_name[i], &img_e[i], &img_emask[i], &xpm_attr);
	assert (IS_PIXMAP(img_e[i]));
	XpmCreatePixmapFromData (r->Xdisplay, r->tabBar.win,
	    xpm_d_name[i], &img_d[i], &img_dmask[i], &xpm_attr);
	assert (IS_PIXMAP(img_d[i]));
#else
	img[i] = XCreatePixmapFromBitmapData (r->Xdisplay,
	    r->tabBar.win, (char *) xbm_name[i], BTN_WIDTH, BTN_HEIGHT,
	    r->tabBar.fg, r->tabBar.bg, XDEPTH);
	assert (IS_PIXMAP(img[i]));
#endif
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "TXT_XOFF=%d, TXT_YOFF=%d, ATAB_EXTRA=%d, TAB_RADIUS=%d\n", TXT_XOFF, TXT_YOFF, ATAB_EXTRA, TAB_RADIUS));
}