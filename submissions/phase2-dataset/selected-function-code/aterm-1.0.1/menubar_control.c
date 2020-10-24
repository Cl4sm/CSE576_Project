void
menubar_control(XButtonEvent * ev)
{
#ifdef MENUBAR
    switch (ev->type) {
    case ButtonPress:
	if (ev->button == Button1)
	    menubar_select(ev);
	break;

    case ButtonRelease:
	if (ev->button == Button1)
	    menu_select(ev);
	break;

    case MotionNotify:
	while (XCheckTypedWindowEvent(Xdisplay, TermWin.parent,
				      MotionNotify,
				      (XEvent *) ev)) ;

	if (ActiveMenu)
	    while (menu_select(ev)) ;
	else
	    ev->y = -1;
	if (ev->y < 0) {
	    Window          unused_root, unused_child;
	    int             unused_root_x, unused_root_y;
	    unsigned int    unused_mask;

	    XQueryPointer(Xdisplay, menuBar.win,
			  &unused_root, &unused_child,
			  &unused_root_x, &unused_root_y,
			  &(ev->x), &(ev->y),
			  &unused_mask);
	    menubar_select(ev);
	}
	break;
    }
#endif
}
