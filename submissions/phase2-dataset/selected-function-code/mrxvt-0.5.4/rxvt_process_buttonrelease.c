void
rxvt_process_buttonrelease(rxvt_t* r, int page, XButtonEvent *ev)
{
    int		 reportmode = 0;

    if ( r->tabClicked != -1 )
    {
	if ( rxvt_is_tabbar_win( r, ev->window ) )
	{
	    rxvt_tabbar_button_release( r, ev );
	    return;
	}
	else
	    /* User dropped a tab somewhere other than the tabbar */
	    r->tabClicked = -1;
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ButtonRelease event\n"));
    r->h->csrO = 0;	/* reset csr Offset */
    if (!r->h->bypass_keystate)
	reportmode = !!ISSET_PMODE(r, page, PrivMode_mouse_report);

#ifdef HAVE_SCROLLBARS
    if (scrollbar_isUpDn())
    {
	scrollbar_setIdle();
	rxvt_scrollbar_update(r, 0);
# ifndef NO_SCROLLBAR_BUTTON_CONTINUAL_SCROLLING
	r->h->refresh_type &= ~SMOOTH_REFRESH;
# endif
    }
#endif	/* HAVE_SCROLLBARS */

#ifdef HAVE_MENUBAR
    if( r->h->showingMenu )
    {
	r->h->showingMenu &= ~POPUP_MENU;
	XDefineCursor( r->Xdisplay, AVTS(r)->vt, r->term_pointer);

	rxvt_menu_select(r, ev);
	return;
    }
#endif

#ifdef SELECTION_SCROLLING
    r->h->pending_scroll_selection=0;
#endif	/* SELECTION_SCROLLING */

    if (ev->window == PVTS(r, page)->vt)
    {
	if (NOT_WIN(ev->subwindow))
	{
	    if (reportmode)
	    {
		/* mouse report from vt window */
		/* don't report release of wheel "buttons" */
		if (ev->button >= 4)
		    return;
#ifdef MOUSE_REPORT_DOUBLECLICK
		/* only report the release of 'slow' single clicks */
		if (
			r->h->MEvent.button != AnyButton
			&& (
			    ev->button != r->h->MEvent.button
			    || (ev->time - r->h->MEvent.time > MULTICLICK_TIME / 2)
			   )
		   )
		{
		    r->h->MEvent.clicks = 0;
		    r->h->MEvent.button = AnyButton;
		    rxvt_mouse_report(r, ev);
		}
#else	/* MOUSE_REPORT_DOUBLECLICK */
		r->h->MEvent.button = AnyButton;
		rxvt_mouse_report(r, ev);
#endif	/* MOUSE_REPORT_DOUBLECLICK */
		return;
	    }
	    /*
	     * dumb hack to compensate for the failure of click-and-drag
	     * when overriding mouse reporting
	     */
	    if (ISSET_PMODE(r, page, PrivMode_mouse_report) &&
		r->h->bypass_keystate &&
		Button1 == ev->button &&
		r->h->MEvent.clicks <= 1)
		rxvt_selection_extend(r, page, ev->x, ev->y, 0);

	    switch (ev->button)
	    {
		case Button1:
		case Button3:
		    rxvt_selection_make(r, page, ev->time);
		    break;
		case Button2:
		    rxvt_selection_request(r, page, ev->time, ev->x, ev->y);
		    break;
#ifdef MOUSE_WHEEL
		case Button4:
		case Button5:
		    rxvt_process_wheel_button (r, page, ev);
		    break;
#endif	/* MOUSE_WHEEL */
	    }
	}
    }
#ifdef HAVE_MENUBAR
    else if (rxvt_is_menubar_win(r, ev->window))
	rxvt_menubar_control(r, ev);
#endif	/* HAVE_MENUBAR */
}