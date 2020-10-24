void
rxvt_process_buttonpress(rxvt_t* r, int page, XButtonEvent *ev)
{
    int			reportmode = 0, clickintime;
    struct rxvt_hidden*	h = r->h;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ButtonPress event\n"));
    h->bypass_keystate = ev->state & (h->ModMetaMask | ShiftMask);
    if (!h->bypass_keystate)
	reportmode = !!ISSET_PMODE(r, page, PrivMode_mouse_report);

    /*
     * VT window processing of button press
     */
    if (ev->window == PVTS(r, page)->vt)
    {
#ifdef HAVE_MENUBAR
	/* Popup menu on Control click on terminal window. */
	if( (ev->state & ControlMask) && !r->h->ActiveMenu)
	{
	    int button;

	    /* In some bizaro world, Button1,2,3 might not be 1,2,3 */
	    switch( ev->button )
	    {
		case Button1:
		    button = 0; break;

		case Button2:
		    button = 1; break;

		case Button3:
		    button = 2; break;

		default:
		    button = -1;
	    }

	    if( button >=0 && r->h->popupMenu[button] )
	    {
		int	x, y;
		Window	unused_cr;

		r->h->showingMenu |= POPUP_MENU;

		XTranslateCoordinates( r->Xdisplay, ev->window,
			r->TermWin.parent, ev->x, ev->y, &x, &y, &unused_cr);

		r->h->ActiveMenu = r->h->popupMenu[button];

		r->h->ActiveMenu->x = x;
		r->h->ActiveMenu->y = y;

		XDefineCursor(r->Xdisplay, AVTS(r)->vt, r->h->bar_pointer);
		rxvt_menu_show(r);
		return;
	    }
	}
#endif

	clickintime = ev->time - h->MEvent.time < MULTICLICK_TIME;
	if (reportmode)
	{
	    /* mouse report from vt window */
	    /* save the xbutton state (for ButtonRelease) */
	    h->MEvent.state = ev->state;
#ifdef MOUSE_REPORT_DOUBLECLICK
	    if (ev->button == h->MEvent.button && clickintime)
	    {
		/* same button, within alloted time */
		h->MEvent.clicks++;
		if (h->MEvent.clicks > 1)
		{
		    /* only report double clicks */
		    h->MEvent.clicks = 2;
		    rxvt_mouse_report(r, ev);

		    /* don't report the release */
		    h->MEvent.clicks = 0;
		    h->MEvent.button = AnyButton;
		}
	    }
	    else
	    {
		/* different button, or time expired */
		h->MEvent.clicks = 1;
		h->MEvent.button = ev->button;
		rxvt_mouse_report(r, ev);
	    }
#else
	    h->MEvent.button = ev->button;
	    rxvt_mouse_report(r, ev);
#endif		    /* MOUSE_REPORT_DOUBLECLICK */
	}
	else
	{
	    if (ev->button != h->MEvent.button)
		h->MEvent.clicks = 0;
	    switch (ev->button)
	    {
		case Button1:
		    if (h->MEvent.button == Button1 && clickintime)
			h->MEvent.clicks++;
		    else
			h->MEvent.clicks = 1;
		    rxvt_selection_click(r, page, h->MEvent.clicks,
			ev->x, ev->y);
		    h->MEvent.button = Button1;
		    break;

		case Button3:
		    if (h->MEvent.button == Button3 && clickintime)
			rxvt_selection_rotate(r, page, ev->x, ev->y);
		    else
			rxvt_selection_extend(r, page, ev->x, ev->y, 1);
		    h->MEvent.button = Button3;
		    break;
	    }
	}
	h->MEvent.time = ev->time;
	return;
    }


    /*
     * Tabbar window processing of button press
     */
    if (rxvt_is_tabbar_win (r, ev->window))
	rxvt_tabbar_dispatcher (r, ev);


#ifdef HAVE_SCROLLBARS
    /*
     * Scrollbar window processing of button press
     */
    if (rxvt_is_scrollbar_win (r, ev->window))
	rxvt_scrollbar_dispatcher (r, page, ev);
#endif


#ifdef HAVE_MENUBAR
    /*
     * Menubar window processing of button press
     */
    if (rxvt_is_menubar_win(r, ev->window))
	rxvt_menubar_control(r, ev);
#endif
}