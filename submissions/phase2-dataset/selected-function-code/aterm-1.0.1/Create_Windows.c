Create_Windows(int argc, char *argv[])
{
    Cursor          cursor;
    XClassHint      classHint;
    XWMHints        wmHint;
    MwmHints 		mwmhints;
    int             i, x, y, flags;
    unsigned int    width, height;
    XSetWindowAttributes attributes;
	unsigned long attr_mask ;

#ifdef PREFER_24BIT
    XWindowAttributes gattr;
        Xcmap = DefaultColormap(Xdisplay, Xscreen);
        Xvisual = DefaultVisual(Xdisplay, Xscreen);

    if (Options & Opt_transparent) {
        XGetWindowAttributes(Xdisplay, RootWindow(Xdisplay, Xscreen), &gattr);
        Xdepth = gattr.depth;
    } else {
        Xdepth = DefaultDepth(Xdisplay, Xscreen);
 /*
  * If depth is not 24, look for a 24bit visual.
  */
        if (Xdepth != 24) {
             XVisualInfo        vinfo;

             if (XMatchVisualInfo(Xdisplay, Xscreen, 24, TrueColor, &vinfo)) {
                Xdepth = 24;
                Xvisual = vinfo.visual;
                Xcmap = XCreateColormap(Xdisplay,
                                        RootWindow(Xdisplay, Xscreen), Xvisual,
                                        AllocNone);
             }
	}
    }
#endif

    if (Options & Opt_borderLess) 
	{
    	if (_XA_MwmAtom == None) {
/*     print_warning("Window Manager does not support MWM hints.  Bypassing window manager control for borderless window.\n");*/
       		attributes.override_redirect = TRUE;
       		mwmhints.flags = 0;
    	} else 
		{
    		mwmhints.flags = MWM_HINTS_DECORATIONS;
			mwmhints.decorations = 0;
    	}
    } else 
	{
    	mwmhints.flags = 0;
    }


/*
 * grab colors before netscape does
 */
    PixColors = &(PixColorsFocused[0]);

    for (i = 0;
	 i < (Xdepth <= 2 ? 2 : NRS_COLORS);
	 i++) {
	const char     *const msg = "can't load color \"%s\", colorID = %d, (%d)";
	XColor          xcol;

	if (!rs_color[i])
	    continue;

	if (!XParseColor(Xdisplay, Xcmap, rs_color[i], &xcol) ||
	    !XAllocColor(Xdisplay, Xcmap, &xcol))
	{
	    print_error(msg, rs_color[i], i, TOTAL_COLORS);
	    rs_color[i] = def_colorName[i];
	    if (!rs_color[i])
		continue;
	    if (!XParseColor(Xdisplay, Xcmap, rs_color[i], &xcol) ||
		!XAllocColor(Xdisplay, Xcmap, &xcol))
	    {
		print_error(msg, rs_color[i], i, TOTAL_COLORS);
		switch (i) {
		case Color_fg:
		case Color_bg:
		/* fatal: need bg/fg color */
		    print_error("aborting");
		    exit(EXIT_FAILURE);
		    break;
#ifndef NO_CURSORCOLOR
		case Color_cursor:
		    xcol.pixel = PixColors[Color_bg];
		    break;
		case Color_cursor2:
		    xcol.pixel = PixColors[Color_fg];
		    break;
#endif				/* NO_CURSORCOLOR */
		case Color_pointer:
		    xcol.pixel = PixColors[Color_fg];
		    break;
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
		case Color_tint:
		    xcol.pixel = PixColors[Color_bg];
		    break;
#endif
		default:
		    xcol.pixel = PixColors[Color_bg];	/* None */
		    break;
		}
		XQueryColor( Xdisplay, Xcmap,&xcol);
	    }
	}
	PixColors[i] = xcol.pixel;
#ifdef OFF_FOCUS_FADING
	PixColorsUnFocused[i] = fade_color(xcol.pixel);
#endif
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
	if( i == Color_tint )
	{
	    TermWin.background.Shading.tintColor.pixel = xcol.pixel ;
	    TermWin.background.Shading.tintColor.red = xcol.red ;
	    TermWin.background.Shading.tintColor.green = xcol.green ;
	    TermWin.background.Shading.tintColor.blue = xcol.blue ;
	    TermWin.background.Shading.tintColor.flags = xcol.flags ;
	}
#endif

    }

    if (Xdepth <= 2 || !rs_color[Color_pointer])
	PixColors[Color_pointer] = PixColors[Color_fg];
    if (Xdepth <= 2 || !rs_color[Color_border])
	PixColors[Color_border] = PixColors[Color_fg];

/*
 * get scrollBar/menuBar shadow colors
 *
 * The calculations of topShadow/bottomShadow values are adapted
 * from the fvwm window manager.
 */
#ifdef KEEP_SCROLLCOLOR
    if (Xdepth <= 2) {		/* Monochrome */
	PixColors[Color_scroll] = PixColors[Color_fg];
	PixColors[Color_topShadow] = PixColors[Color_bg];
	PixColors[Color_bottomShadow] = PixColors[Color_bg];
    } else {
	XColor          xcol, white;

    /* bottomShadowColor */
	xcol.pixel = PixColors[Color_scroll];
	XQueryColor(Xdisplay, Xcmap, &xcol);

	xcol.red = ((xcol.red) / 2);
	xcol.green = ((xcol.green) / 2);
	xcol.blue = ((xcol.blue) / 2);

	if (!XAllocColor(Xdisplay, Xcmap, &xcol)) {
	    print_error("can't allocate %s", "Color_bottomShadow");
	    xcol.pixel = PixColors[minCOLOR];
	}
	PixColors[Color_bottomShadow] = xcol.pixel;

    /* topShadowColor */
# ifdef PREFER_24BIT
	white.red = white.green = white.blue = (unsigned short) ~0;
	XAllocColor(Xdisplay, Xcmap, &white);
/*        XFreeColors(Xdisplay, Xcmap, &white.pixel, 1, ~0); */
# else
	white.pixel = WhitePixel(Xdisplay, Xscreen);
	XQueryColor(Xdisplay, Xcmap, &white);
# endif

	xcol.pixel = PixColors[Color_scroll];
	XQueryColor(Xdisplay, Xcmap, &xcol);

	xcol.red = max((white.red / 5), (int)xcol.red);
	xcol.green = max((white.green / 5), (int)xcol.green);
	xcol.blue = max((white.blue / 5), (int)xcol.blue);

	xcol.red = min((int)white.red, (xcol.red * 7) / 5);
	xcol.green = min((int)white.green, (xcol.green * 7) / 5);
	xcol.blue = min((int)white.blue, (xcol.blue * 7) / 5);

	if (!XAllocColor(Xdisplay, Xcmap, &xcol)) {
	    print_error("can't allocate %s", "Color_topShadow");
	    xcol.pixel = PixColors[Color_White];
	}
	PixColors[Color_topShadow] = xcol.pixel;
    }
#endif				/* KEEP_SCROLLCOLOR */

    szHint.base_width = (2 * TermWin_internalBorder +
			 (Options & Opt_scrollBar ? (SB_WIDTH + 2 * sb_shadow)
			  : 0));
    szHint.base_height = (2 * TermWin_internalBorder);

    flags = (rs_geometry ?
	     XParseGeometry(rs_geometry, &x, &y, &width, &height) : 0);

    if (flags & WidthValue) {
	szHint.width = width;
	szHint.flags |= USSize;
    }
    if (flags & HeightValue) {
	szHint.height = height;
	szHint.flags |= USSize;
    }

    set_terminal_size( szHint.width, szHint.height );

    change_font(1, NULL);

    { /* ONLYIF MENUBAR */
	szHint.base_height += (delay_menu_drawing ? menuBar_TotalHeight() : 0);
    }

    if (flags & XValue) {
	if (flags & XNegative) {
	    x += (DisplayWidth(Xdisplay, Xscreen)
		  - (szHint.width + TermWin_internalBorder));
	    szHint.win_gravity = NorthEastGravity;
	}
	szHint.x = x;
	szHint.flags |= USPosition;
    }
    if (flags & YValue) {
	if (flags & YNegative) {
	    y += (DisplayHeight(Xdisplay, Xscreen)
		  - (szHint.height + TermWin_internalBorder));
	    szHint.win_gravity = (szHint.win_gravity == NorthEastGravity ?
				  SouthEastGravity : SouthWestGravity);
	}
	szHint.y = y;
	szHint.flags |= USPosition;
    }
/* parent window - reverse video so we can see placement errors
 * sub-window placement & size in resize_subwindows()
 */
    attributes.background_pixel = PixColors[Color_bg];
    attributes.border_pixel = PixColors[Color_fg];
	attributes.event_mask = ( KeyPressMask | 
							  FocusChangeMask |
		  					  StructureNotifyMask | 
							  VisibilityChangeMask |
							  PropertyChangeMask);
    attributes.colormap = Xcmap;
	attributes.background_pixmap = ParentRelative;
	attr_mask = CWBorderPixel | CWEventMask ; 
#ifdef HAVE_AFTERSTEP
	if( TermWin.background.trgType == BGT_MyStyle )
		attr_mask |= CWBackPixmap ; 
#endif		
#ifdef TRANSPARENT
	if( get_flags(Options, Opt_transparent) )
		attr_mask |= CWBackPixmap ; 
#endif
	if( (attr_mask & CWBackPixmap ) == 0 )
	  	attr_mask |= CWBackPixel ; 	 

#ifdef HAVE_AFTERIMAGE
    TermWin.parent = create_visual_window( asv, Xroot, 
					   szHint.x, szHint.y,
					   szHint.width, szHint.height,
                       TermWin.borderWidth,
					   InputOutput,
					   attr_mask,
					   &attributes);

#else
	attr_mask |= CWColormap ;
    TermWin.parent = XCreateWindow(Xdisplay, Xroot,
				   szHint.x, szHint.y,
				   szHint.width, szHint.height,
                                   TermWin.borderWidth,
				   Xdepth, InputOutput,
				   Xvisual,
				   attr_mask,
				   &attributes);
#endif
    TermWin.bMapped = 0 ;
    ParentWin[0] = TermWin.parent ;
    ParentWinNum = 1 ;

    xterm_seq(XTerm_title, rs_title);
    xterm_seq(XTerm_iconName, rs_iconName);
/* ignore warning about discarded `const' */
    classHint.res_name = (char *)rs_name;
    classHint.res_class = APL_CLASS;
    wmHint.input = True;
    wmHint.initial_state = (Options & Opt_iconic ? IconicState : NormalState);
    wmHint.window_group = TermWin.parent;
    wmHint.flags = (InputHint | StateHint | WindowGroupHint);

    XSetWMProperties(Xdisplay, TermWin.parent, NULL, NULL, argv, argc,
		     &szHint, &wmHint, &classHint);

	/* publish our PID : */
	{
		long ldata = getpid();
        XChangeProperty (Xdisplay, TermWin.parent, _XA_NET_WM_PID, XA_CARDINAL, 32,
                         PropModeReplace, (unsigned char *)&ldata, 1);
	}


    if (mwmhints.flags && _XA_MwmAtom) {
    	XChangeProperty(Xdisplay, TermWin.parent, _XA_MwmAtom, _XA_MwmAtom, 32, PropModeReplace, (unsigned char *) &mwmhints, PROP_MWM_HINTS_ELEMENTS);
    }
   

/* vt cursor: Black-on-White is standard, but this is more popular */
    TermWin_cursor = XCreateFontCursor(Xdisplay, XC_xterm);
    {
		XColor          fg, bg;

		fg.pixel = PixColors[Color_pointer];
		XQueryColor(Xdisplay, Xcmap, &fg);
		bg.pixel = PixColors[Color_bg];
		XQueryColor(Xdisplay, Xcmap, &bg);
		XRecolorCursor(Xdisplay, TermWin_cursor, &fg, &bg);
    }

/* cursor (menuBar/scrollBar): Black-on-White */
    cursor = XCreateFontCursor(Xdisplay, XC_left_ptr);
	attributes.event_mask = (ExposureMask | 
							 ButtonPressMask | 
							 ButtonReleaseMask |
		  					 Button1MotionMask | 
							 Button3MotionMask);
	attributes.cursor = TermWin_cursor ;
/* the vt window */
#ifdef HAVE_AFTERIMAGE
    TermWin.vt = create_visual_window( asv, TermWin.parent,
				     	0, 0,
				     	szHint.width, szHint.height,
					   	0,
					   	InputOutput,
					   	attr_mask|CWCursor,
					   	&attributes);
#else
    TermWin.vt = XCreateSimpleWindow(Xdisplay, TermWin.parent,
				     0, 0,
				     szHint.width, szHint.height,
				     0,
				     PixColors[Color_fg],
				     PixColors[Color_bg]);
    XSelectInput(Xdisplay, TermWin.vt, attributes.event_mask );
	XDefineCursor(Xdisplay, TermWin.vt, TermWin_cursor);
#endif
    

#ifdef TRANSPARENT
    if (get_flags(Options, Opt_transparent)) {
		if( TermWin.background.trgType != BGT_None )
		    SetSrcPixmap(GetRootPixmap(None));
		else
			XSetWindowBackgroundPixmap(Xdisplay, TermWin.vt, ParentRelative);
    }
#endif
    /* added by Sasha Vasko to enabling Tracking of the Background changes */
#if defined(TRANSPARENT)||defined(_MYSTYLE_)
#if !defined(_MYSTYLE_)
    if ((Options & Opt_transparent)||(Options&Opt_transparent_sb))
#endif
	XSelectInput( Xdisplay, Xroot, PropertyChangeMask );

#endif

/* scrollBar: size doesn't matter */
#ifdef HAVE_AFTERIMAGE
    scrollBar.win = create_visual_window( asv, TermWin.parent,
						0, 0,
						1, 1,
						0,
					   	InputOutput,
					   	CWBackPixel | CWBorderPixel,
					   	&attributes);
#else    
	scrollBar.win = XCreateSimpleWindow(Xdisplay, TermWin.parent,
						0, 0,
						1, 1,
						0,
					PixColors[Color_fg],
					PixColors[Color_bg]);
#endif

    XDefineCursor(Xdisplay, scrollBar.win, cursor);
    XSelectInput(Xdisplay, scrollBar.win,
		 (ExposureMask | ButtonPressMask | ButtonReleaseMask |
		  Button1MotionMask | Button2MotionMask | Button3MotionMask)
	);

    { /* ONLYIF MENUBAR */
	create_menuBar(cursor);
    }
#if defined(BACKGROUND_IMAGE)
    if (rs_backgroundPixmap != NULL
#ifdef TRANSPARENT
         && !(Options & Opt_transparent)
#endif
       )
        SetBackgroundPixmap((char*)rs_backgroundPixmap);
#endif
/* graphics context for the vt window */
    {
	XGCValues       gcvalue;
	gcvalue.font = TermWin.font->fid;
	gcvalue.foreground = PixColors[Color_fg];
	gcvalue.background = PixColors[Color_bg];
	gcvalue.function = ParseGCType(rs_textType, GXcopy);
	gcvalue.graphics_exposures = 0;
#ifdef HAVE_AFTERIMAGE
	TermWin.gc = create_visual_gc(asv, TermWin.vt,
			       GCFunction|
			       GCForeground | GCBackground |
			       GCFont | GCGraphicsExposures,
			       &gcvalue);
#else
	TermWin.gc = XCreateGC(Xdisplay, TermWin.vt,
			       GCFunction|
			       GCForeground | GCBackground |
			       GCFont | GCGraphicsExposures,
			       &gcvalue);
#endif
#if defined(BACKGROUND_IMAGE) || defined(TRANSPARENT)
	if( rs_color[Color_tint] )
        {
	    if( rs_tintType )
	    {
		if( strcmp( rs_tintType, TINT_TYPE_TRUE) == 0 )
		{
		    TermWin.tintGC = None ;
		    if( TermWin.background.trgType == BGT_None )
				TermWin.background.trgType = BGT_Cut ;
		}

	    }
	    if( TermWin.background.trgType == BGT_None )
	    {
	        gcvalue.function = ParseGCType(rs_tintType, GXand);
	        gcvalue.foreground = PixColors[Color_tint];
#ifdef HAVE_AFTERIMAGE
    	        TermWin.tintGC = create_visual_gc( asv, TermWin.vt,
							    			GCFunction|GCForeground|GCGraphicsExposures,
											&gcvalue);
#else
    	        TermWin.tintGC = XCreateGC(	Xdisplay, TermWin.vt,
							    			GCFunction|GCForeground|GCGraphicsExposures,
											&gcvalue);
#endif
	    }
        }
#endif
    }
}
