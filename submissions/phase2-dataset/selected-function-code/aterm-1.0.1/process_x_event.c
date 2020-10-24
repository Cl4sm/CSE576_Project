process_x_event(XEvent * ev)
{
    static int      bypass_keystate = 0;
    int             reportmode;
    static int      csrO = 0;	/* Hops - csr offset in thumb/slider      */


#if !defined(NO_DEBUG_OUTPUT)
   	fprintf(stderr, "****************************************************************\n");
   	fprintf(stderr, "%s:%s:%d(%ld)><<EVENT type(%d(%s))->x.window(%lx)->send_event(%d)\n", __FILE__, __FUNCTION__, __LINE__, time(NULL), ev->type, event_type2name(ev->type), ev->xany.window, ev->xany.send_event);
#endif

/*        to give proper Scroll behaviour */
    switch (ev->type) {
    case KeyPress:
	lookup_key(ev);
	break;

	case DestroyNotify :
		if( ev->xdestroywindow.window == TermWin.vt || ev->xdestroywindow.window == TermWin.parent )
		{
#ifdef __CYGWIN__
    	/* cygwin does not kill shell properly */
		/* fprintf( stderr, "cmd_pid = %d\n", cmd_pid ); */
			kill( cmd_pid, SIGKILL );
#endif	  
		    exit(EXIT_SUCCESS);
		}	 
	    break ;

    case ClientMessage:
	if (ev->xclient.format == 32 && ev->xclient.data.l[0] == wmDeleteWindow)
	{
#ifdef __CYGWIN__
    	/* cygwin does not kill shell properly */
		/* fprintf( stderr, "cmd_pid = %d\n", cmd_pid ); */
		kill( cmd_pid, SIGKILL );
#endif	  
	    exit(EXIT_SUCCESS);
	}
#ifdef OFFIX_DND
    /* OffiX Dnd (drag 'n' drop) protocol */
	if (ev->xclient.message_type == DndProtocol &&
	    ((ev->xclient.data.l[0] == DndFile) ||
	     (ev->xclient.data.l[0] == DndDir) ||
	     (ev->xclient.data.l[0] == DndLink))) {
	/* Get Dnd data */
	    Atom            ActualType;
	    int             ActualFormat;
	    unsigned char  *data;
	    unsigned long   Size, RemainingBytes;

	    XGetWindowProperty(Xdisplay, Xroot,
			       DndSelection,
			       0L, 1000000L,
			       False, AnyPropertyType,
			       &ActualType, &ActualFormat,
			       &Size, &RemainingBytes,
			       &data);
	    XChangeProperty(Xdisplay, Xroot,
			    XA_CUT_BUFFER0, XA_STRING,
			    8, PropModeReplace,
			    data, strlen(data));
	    selection_paste(Xroot, XA_CUT_BUFFER0, True);
	    XSetInputFocus(Xdisplay, Xroot, RevertToNone, CurrentTime);
	}
#endif				/* OFFIX_DND */
	break;

    case MappingNotify:
	XRefreshKeyboardMapping(&(ev->xmapping));
	break;

    /* Here's my conclusiion:
     * If the window is completely unobscured, use bitblt's
     * to scroll. Even then, they're only used when doing partial
     * screen scrolling. When partially obscured, we have to fill
     * in the GraphicsExpose parts, which means that after each refresh,
     * we need to wait for the graphics expose or Noexpose events,
     * which ought to make things real slow!
     */
    case VisibilityNotify:
	switch (ev->xvisibility.state) {
	case VisibilityUnobscured:
	    refresh_type = FAST_REFRESH;
	    break;

	case VisibilityPartiallyObscured:
	    refresh_type = SLOW_REFRESH;
	    break;

	default:
	    refresh_type = NO_REFRESH;
	    break;
	}
	break;

    case FocusIn:
	if (!TermWin.focus) {
	    TermWin.focus = 1;
#ifdef OFF_FOCUS_FADING
	    if( rs_fade != NULL )
	    {
    		PixColors = &(PixColorsFocused[0]);
		on_colors_changed(Color_bg);
	    }
#endif
#if  !defined(USE_XIM) && !defined(NO_XLOCALE)
	    if (Input_Context != NULL)
		XSetICFocus(Input_Context);
#endif
	}
	break;

    case FocusOut:
	if (TermWin.focus) {
	    TermWin.focus = 0;
#ifdef OFF_FOCUS_FADING
	    if( rs_fade != NULL )
	    {
		PixColors = &(PixColorsUnFocused[0]);
		on_colors_changed(Color_bg);
	    }
#endif
#if  !defined(USE_XIM) && !defined(NO_XLOCALE)
	    if (Input_Context != NULL)
		XUnsetICFocus(Input_Context);
#endif
	}
	break;

    case ConfigureNotify:
	while( XCheckTypedWindowEvent( Xdisplay, ev->xconfigure.window, ConfigureNotify, ev ) );
	resize_window(ev);
	menubar_expose();
	break;

    case SelectionClear:
	selection_clear();
	break;

    case SelectionNotify:
	selection_paste(ev->xselection.requestor, ev->xselection.property, True);
	break;

    case SelectionRequest:
	selection_send(&(ev->xselectionrequest));
	break;

     case PropertyNotify:
#ifdef DEBUG_BACKGROUND_PMAP    	
		{
			char *prop_name = XGetAtomName( Xdisplay, ev->xproperty.atom );
        	fprintf( stderr, "PropertyNotify : %s(%lX)\n", prop_name, ev->xproperty.atom );
			if( prop_name )
	    		XFree( prop_name );
    	}
#endif	 	
		if( ev->xproperty.window == Xroot ) 
		{	
#ifdef HAVE_AFTERSTEP
			if (ev->xproperty.atom == _AS_STYLE ) 
			{
				const char *mystyle_name ;
	    		if( rs_mystyle )
					mystyle_name = rs_mystyle ;
	    		else
					mystyle_name = GetDefaultMyStyle() ;

				mystyle_handle_property_event( ev );
	    		set_mystyle( mystyle_find( mystyle_name ), True );
	    		break;
			}else if( ev->xproperty.atom  == _XROOTPMAP_ID )
			{	
				if( TermWin.background.trgType == BGT_MyStyle )
				{	
					if(	!TransparentMS(TermWin.background.mystyle) )
					{
#ifdef DEBUG_BACKGROUND_PMAP			
						fprintf( stderr, "texture type = %d\n", TermWin.background.mystyle->texture_type );
#endif
	    				break ;
					}
					destroy_asimage( &Scr.RootImage );
				}
			}
#endif
#ifdef TRANSPARENT
			if( ev->xproperty.atom  == _XROOTPMAP_ID )
			{
				if( IsTransparentPixmap() ) 	   
				{	
					Pixmap p;
					if( read_32bit_property (Xroot, _XROOTPMAP_ID, &p) )
					{	
						if( p != TermWin.background.srcPixmap ) 
							SetSrcPixmap( p );
					}else 
						ValidateSrcPixmap(True);	  
				
#ifdef DEBUG_BACKGROUND_PMAP			   
					fprintf( stderr, "root pmap changed to = %lX\n", TermWin.background.srcPixmap );
#endif

					if( TransparentPixmapNeedsUpdate() ) 
						request_background_update();
				}else if( get_flags(Options, Opt_transparent) )
				{
			        scr_clear();
	    		    scr_touch();
				}	 
			}
#endif
			if( ev->xproperty.atom  == _XA_NET_SUPPORTING_WM_CHECK )
			{	
				check_extended_wm_hints_support();
#ifdef DEBUG_BACKGROUND_PMAP			   
				fprintf( stderr, "WM change. Desktops are %ssupported\n", get_flags( ExtWM.flags, WM_SupportsDesktops )?"":"not " );
#endif
			}else if( ev->xproperty.atom  == _XA_NET_CURRENT_DESKTOP )
			{
				if( !read_32bit_property (Xroot, _XA_NET_CURRENT_DESKTOP, &ExtWM.current_desktop) )
					clear_flags( ExtWM.flags, WM_SupportsDesktops );
				else if( get_flags( ExtWM.flags, WM_ClaimSupportsDesktops ) )
					set_flags( ExtWM.flags, WM_SupportsDesktops	);

				
#ifdef DEBUG_BACKGROUND_PMAP			   
				fprintf( stderr, "Curr Desk change to %ld. Desktops are %ssupported\n", ExtWM.current_desktop, get_flags( ExtWM.flags, WM_SupportsDesktops )?"":"not " );
#endif
				/* Don't do it here - wait for background change : 
				 * if( TransparentPixmapNeedsUpdate() ) 
					request_background_update();
				 */
			}	 
		}
		if( ev->xproperty.window == TermWin.parent ) 
		{
			if( ev->xproperty.atom  == _XA_NET_WM_DESKTOP )
			{
#ifdef DEBUG_BACKGROUND_PMAP			   
				fprintf( stderr, "Aterm Desk change from %ld. Desktops are %ssupported\n", ExtWM.aterm_desktop, get_flags( ExtWM.flags, WM_SupportsDesktops )?"":"not " );
#endif
				if( !read_32bit_property (TermWin.parent, _XA_NET_WM_DESKTOP, &ExtWM.aterm_desktop) )
					clear_flags( ExtWM.flags, WM_SupportsDesktops );
#ifdef DEBUG_BACKGROUND_PMAP			   
				fprintf( stderr, "Aterm Desk change to %ld. Desktops are %ssupported\n", ExtWM.aterm_desktop, get_flags( ExtWM.flags, WM_SupportsDesktops )?"":"not " );
#endif
				if( TransparentPixmapNeedsUpdate() ) 
					request_background_update();
			}else if( ev->xproperty.atom  == _XA_NET_WM_STATE )
			{
				if( check_extended_wm_state() ) 
					if( TransparentPixmapNeedsUpdate() ) 
						request_background_update();
#ifdef DEBUG_BACKGROUND_PMAP			   
				fprintf( stderr, "Aterm state change to: %sSticky, %sShaded, %sHidden\n", 
								 get_flags( ExtWM.flags, WM_AtermStateSticky )?"":"not ",
								 get_flags( ExtWM.flags, WM_AtermStateShaded )?"":"not ",
								 get_flags( ExtWM.flags, WM_AtermStateHidden )?"":"not " );
#endif
			
			}	 
		}	 
		break;
     case UnmapNotify:
        TermWin.bMapped = 0 ;
/*	fprintf(stderr, "\n aterm is UnMapped(event)");*/
	break;

     case MapNotify:
/*        {
	  XWindowAttributes attr ;

	    XGetWindowAttributes( Xdisplay, ParentWin[0], &attr );
	    TermWin.bMapped = (attr.map_state == IsViewable)?1:0 ;

	    fprintf(stderr, "\n aterm is %s", (TermWin.bMapped)?"Mapped":"UnMapped");
	}*/
/*        fprintf(stderr, "\n aterm is %s", (TermWin.bMapped)?"Mapped":"UnMapped");*/
	TermWin.bMapped = 1 ;
#ifdef TRANSPARENT
	if ( TransparentPixmapNeedsUpdate() )
		request_background_update();
#endif
#if 0
	refresh_transparent_scrollbar();
	if( Options & Opt_transparent)
	{
	  Pixmap tmp = GetRootPixmap(None);
	    if( tmp != TermWin.background.srcPixmap && TermWin.background.srcPixmap != None )
	    {
	        if( TermWin.background.trgType != BGT_None )
	        {
	            SetSrcPixmap(tmp);
	            RenderPixmap(0);
	        }
	        scr_clear();
	        scr_touch();
	    }
	}
#endif
	break ;
     case ReparentNotify:
#ifdef TRANSPARENT
         {
	    int n;
	    XWindowAttributes attr ;
            int (*oldXErrorHandler) (Display *, XErrorEvent *) =
		  XSetErrorHandler (pixmap_error_handler);

	    for( n = 1 ; n < PARENTS_NUM ; n ++ )    ParentWin[n] = None;
	    /*
	     * Make the frame window set by the window manager have
	     * the root background. Some window managers put few nested frame
	     * windows for each client, so we have to take care about that.
	     */
	    for( ParentWinNum = 1 ;ParentWinNum<PARENTS_NUM; ParentWinNum++)
	    {
		Window root;
		Window parent;
		Window *list;

		XQueryTree(Xdisplay, ParentWin[ParentWinNum-1], &root, &parent, &list, &n);
		XFree(list);
   		if (parent == Xroot) break;
	        ParentWin[ParentWinNum] = parent;
		/* we want to get all map/unmap events from this window as well*/
	/*	XSelectInput(Xdisplay, parent, StructureNotifyMask); */

		if (Options & Opt_transparent)
		    XSetWindowBackgroundPixmap(Xdisplay, parent, ParentRelative);
	    }
	    if( XGetWindowAttributes( Xdisplay, ParentWin[0], &attr ))
		TermWin.bMapped = (attr.map_state == IsViewable)?1:0 ;

	    XSetErrorHandler (oldXErrorHandler);
    	    scr_clear();
	}
#endif /* TRANSPARENT */
	break;
    case GraphicsExpose:
    case Expose:
	if (ev->xany.window == TermWin.vt) {
#if !defined(NO_DEBUG_OUTPUT)
   	fprintf(stderr, "exposed area = %dx%d%+d%+d\n", ev->xexpose.x, ev->xexpose.y,
		       										ev->xexpose.width, ev->xexpose.height);
#endif
	    scr_expose(ev->xexpose.x, ev->xexpose.y,
		       ev->xexpose.width, ev->xexpose.height);
	} else {
	    XEvent          unused_xevent;

	    while (XCheckTypedWindowEvent(Xdisplay, ev->xany.window,
					  Expose,
					  &unused_xevent)) ;
	    while (XCheckTypedWindowEvent(Xdisplay, ev->xany.window,
					  GraphicsExpose,
					  &unused_xevent)) ;
	    if (isScrollbarWindow(ev->xany.window)) {
		scrollbar_setNone();
		scrollbar_show(0);
	    }
	    if (menubar_visible() && isMenuBarWindow(ev->xany.window))
		menubar_expose();
	    Gr_expose(ev->xany.window);
	}
	break;

    case ButtonPress:
	bypass_keystate = (ev->xbutton.state & (Mod1Mask | ShiftMask));
	reportmode = (bypass_keystate ?
		      0 : (PrivateModes & PrivMode_mouse_report));

	if (ev->xany.window == TermWin.vt) {
	    if (ev->xbutton.subwindow != None)
		Gr_ButtonPress(ev->xbutton.x, ev->xbutton.y);
	    else {
		if (reportmode) {
		/* mouse report from vt window */
		/* save the xbutton state (for ButtonRelease) */
		    MEvent.state = ev->xbutton.state;
#ifdef MOUSE_REPORT_DOUBLECLICK
		    if (ev->xbutton.button == MEvent.button
		     && (ev->xbutton.time - MEvent.time < MULTICLICK_TIME)) {
		    /* same button, within alloted time */
			MEvent.clicks++;
			if (MEvent.clicks > 1) {
			/* only report double clicks */
			    MEvent.clicks = 2;
			    mouse_report(&(ev->xbutton));

			/* don't report the release */
			    MEvent.clicks = 0;
			    MEvent.button = AnyButton;
			}
		    } else {
		    /* different button, or time expired */
			MEvent.clicks = 1;
			MEvent.button = ev->xbutton.button;
			mouse_report(&(ev->xbutton));
		    }
#else
		    MEvent.button = ev->xbutton.button;
		    mouse_report(&(ev->xbutton));
#endif				/* MOUSE_REPORT_DOUBLECLICK */
		} else {
		    if (ev->xbutton.button != MEvent.button)
			MEvent.clicks = 0;
		    switch (ev->xbutton.button) {
		    case Button1:
			if (MEvent.button == Button1
			&& (ev->xbutton.time - MEvent.time < MULTICLICK_TIME))
			    MEvent.clicks++;
			else
			    MEvent.clicks = 1;
			selection_click(MEvent.clicks, ev->xbutton.x,
					ev->xbutton.y);
			MEvent.button = Button1;
			break;

		    case Button3:
			if (MEvent.button == Button3
			&& (ev->xbutton.time - MEvent.time < MULTICLICK_TIME))
			    selection_rotate(ev->xbutton.x, ev->xbutton.y);
			else
			    selection_extend(ev->xbutton.x, ev->xbutton.y, 1);
			MEvent.button = Button3;
			break;
		    }
		}
		MEvent.time = ev->xbutton.time;
		return;
	    }
	}
	if (isScrollbarWindow(ev->xany.window)) {
	    scrollbar_setNone();
	/*
	 * Rxvt-style scrollbar:
	 * move up if mouse is above slider
	 * move dn if mouse is below slider
	 *
	 * XTerm-style scrollbar:
	 * Move display proportional to pointer location
	 * pointer near top -> scroll one line
	 * pointer near bot -> scroll full page
	 */
#ifndef NO_SCROLLBAR_REPORT
	    if (reportmode) {
	    /*
	     * Mouse report disabled scrollbar:
	     * arrow buttons - send up/down
	     * click on scrollbar - send pageup/down
	     */
		if (scrollbar_upButton(ev->xbutton.y))
		    tt_printf((unsigned char *) "\033[A");
		else if (scrollbar_dnButton(ev->xbutton.y))
		    tt_printf((unsigned char *) "\033[B");
		else
		    switch (ev->xbutton.button) {
		    case Button2:
			tt_printf((unsigned char *) "\014");
			break;
		    case Button1:
			tt_printf((unsigned char *) "\033[6~");
			break;
		    case Button3:
			tt_printf((unsigned char *) "\033[5~");
			break;
		    }
	    } else
#endif				/* NO_SCROLLBAR_REPORT */
	    {
		if (scrollbar_upButton(ev->xbutton.y)) {
#ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
		    scroll_arrow_delay = SCROLLBAR_INITIAL_DELAY;
#endif
		    if (scr_page(UP, 1))
			scrollbar_setUp();
		} else if (scrollbar_dnButton(ev->xbutton.y)) {
#ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
		    scroll_arrow_delay = SCROLLBAR_INITIAL_DELAY;
#endif
		    if (scr_page(DN, 1))
			scrollbar_setDn();
		} else
		    switch (ev->xbutton.button) {
		    case Button2:
#ifndef FUNKY_SCROLL_BEHAVIOUR
			csrO = (scrollBar.bot - scrollBar.top) / 2;
		    /* align to thumb center */
#else
# ifndef XTERM_SCROLLBAR
			if (scrollbar_above_slider(ev->xbutton.y) ||
			    scrollbar_below_slider(ev->xbutton.y))
# endif
#endif				/* !FUNKY_SCROLL_BEHAVIOUR */
			    scr_move_to(scrollbar_position(ev->xbutton.y) - csrO,
					scrollbar_size());
			scrollbar_setMotion();
			break;

		    case Button1:
#ifndef FUNKY_SCROLL_BEHAVIOUR
			csrO = ev->xbutton.y - scrollBar.top;
		    /* ptr ofset in thumb */
#endif
		    /*drop */

		    case Button3:
#ifndef XTERM_SCROLLBAR
			if (scrollbar_above_slider(ev->xbutton.y))
# ifdef RXVT_SCROLL_FULL
			    scr_page(UP, TermWin.nrow - 1);
# else
			    scr_page(UP, TermWin.nrow / 4);
# endif
			else if (scrollbar_below_slider(ev->xbutton.y))
# ifdef RXVT_SCROLL_FULL
			    scr_page(DN, TermWin.nrow - 1);
# else
			    scr_page(DN, TermWin.nrow / 4);
# endif
			else
			    scrollbar_setMotion();
#else				/* XTERM_SCROLLBAR */
			scr_page((ev->xbutton.button == Button1 ? DN : UP),
				 (TermWin.nrow *
				  scrollbar_position(ev->xbutton.y) /
				  scrollbar_size())
			    );
#endif				/* XTERM_SCROLLBAR */
			break;
		    }
	    }
	    return;
	}
	if (isMenuBarWindow(ev->xany.window)) {
	    menubar_control(&(ev->xbutton));
	    return;
	}
	break;

    case ButtonRelease:
	csrO = 0;		/* reset csr Offset */
	reportmode = (bypass_keystate ?
		      0 : (PrivateModes & PrivMode_mouse_report));

	if (scrollbar_isUpDn()) {
	    scrollbar_setNone();
	    scrollbar_show(0);
#ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
	    refresh_type &= ~SMOOTH_REFRESH;
#endif
	}
	if (ev->xany.window == TermWin.vt) {
	    if (ev->xbutton.subwindow != None)
		Gr_ButtonRelease(ev->xbutton.x, ev->xbutton.y);
	    else {
		if (reportmode) {
		/* Don't report release events for the mouse wheel */
		if (ev->xbutton.button == Button4 || ev->xbutton.button == Button5)
		    return;
		/* mouse report from vt window */
#ifdef MOUSE_REPORT_DOUBLECLICK
		/* only report the release of 'slow' single clicks */
		    if (MEvent.button != AnyButton &&
			(ev->xbutton.button != MEvent.button ||
		      (ev->xbutton.time - MEvent.time > MULTICLICK_TIME / 2))
			) {
			MEvent.clicks = 0;
			MEvent.button = AnyButton;
			mouse_report(&(ev->xbutton));
		    }
#else				/* MOUSE_REPORT_DOUBLECLICK */
		    MEvent.button = AnyButton;
		    mouse_report(&(ev->xbutton));
#endif				/* MOUSE_REPORT_DOUBLECLICK */
		    return;
		}
	    /*
	     * dumb hack to compensate for the failure of click-and-drag
	     * when overriding mouse reporting
	     */
		if ((PrivateModes & PrivMode_mouse_report) &&
		    (bypass_keystate) &&
		    (ev->xbutton.button == Button1) &&
		    (MEvent.clicks <= 1))
		    selection_extend(ev->xbutton.x, ev->xbutton.y, 0);

		switch (ev->xbutton.button) {
		case Button1:
		case Button3:
		    selection_make(ev->xbutton.time, ev->xbutton.state);
		    break;

		case Button2:
		    selection_request(ev->xbutton.time,
				      ev->xbutton.x, ev->xbutton.y);
		    break;
#ifndef NO_MOUSE_WHEEL
		case Button4:
		    scr_page(UP, (ev->xbutton.state & ShiftMask) ? 1 : 5);
		    break;
		case Button5:
		    scr_page(DN, (ev->xbutton.state & ShiftMask) ? 1 : 5);
		    break;
#endif
		}
	    }
	} else if (isMenuBarWindow(ev->xany.window)) {
	    menubar_control(&(ev->xbutton));
	}
	break;

    case MotionNotify:
	if (isMenuBarWindow(ev->xany.window)) {
	    menubar_control(&(ev->xbutton));
	    break;
	}
	if ((PrivateModes & PrivMode_mouse_report) && !(bypass_keystate))
	    break;

	if (ev->xany.window == TermWin.vt) {
	    if ((ev->xbutton.state & (Button1Mask | Button3Mask))) {
		Window          unused_root, unused_child;
		int             unused_root_x, unused_root_y;
		unsigned int    unused_mask;

		while (XCheckTypedWindowEvent(Xdisplay, TermWin.vt,
					      MotionNotify, ev)) ;
		XQueryPointer(Xdisplay, TermWin.vt,
			      &unused_root, &unused_child,
			      &unused_root_x, &unused_root_y,
			      &(ev->xbutton.x), &(ev->xbutton.y),
			      &unused_mask);
#ifdef MOUSE_THRESHOLD
	    /* deal with a `jumpy' mouse */
		if ((ev->xmotion.time - MEvent.time) > MOUSE_THRESHOLD)
#endif
		    selection_extend((ev->xbutton.x), (ev->xbutton.y),
				  (ev->xbutton.state & Button3Mask) ? 2 : 0);
	    }
	} else if ((ev->xany.window == scrollBar.win) && scrollbar_isMotion()) {
	    Window          unused_root, unused_child;
	    int             unused_root_x, unused_root_y;
	    unsigned int    unused_mask;

	    while (XCheckTypedWindowEvent(Xdisplay, scrollBar.win,
					  MotionNotify, ev)) ;
	    XQueryPointer(Xdisplay, scrollBar.win,
			  &unused_root, &unused_child,
			  &unused_root_x, &unused_root_y,
			  &(ev->xbutton.x), &(ev->xbutton.y),
			  &unused_mask);
	    scr_move_to(scrollbar_position(ev->xbutton.y) - csrO,
			scrollbar_size());
	    scr_refresh(refresh_type);
	    refresh_count = refresh_limit = 0;
	    scrollbar_show(1);
#ifdef USE_XIM
            IMSendSpot();
#endif
	}
	break;
    }
}
