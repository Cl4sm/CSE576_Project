void
rxvt_process_motionnotify (rxvt_t* r, XEvent* ev)
{
    int		    page = ATAB(r);
    Window	    unused_root, unused_child;
    int		    unused_root_x, unused_root_y;
    unsigned int    unused_mask;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "MotionNotify event\n"));
#ifdef POINTER_BLANK
    if (ISSET_OPTION(r, Opt_pointerBlank) && PVTS(r, page)->hidden_pointer)
	rxvt_pointer_unblank (r, page);
#endif
#ifdef HAVE_MENUBAR
    if (rxvt_is_menubar_win(r, ev->xmotion.window))
    {
	rxvt_menubar_control(r, &(ev->xbutton));
	return;
    }

    if (r->h->showingMenu )
    {
	rxvt_menu_select( r, &(ev->xbutton));
	return;
    }
#endif

    if (ISSET_PMODE(r, page, PrivMode_mouse_report) &&
	!(r->h->bypass_keystate))
	return;

    if (ev->xany.window == PVTS(r, page)->vt)
    {
	if ((ev->xbutton.state & (Button1Mask | Button3Mask)))
	{
	    while (XCheckTypedWindowEvent(r->Xdisplay, PVTS(r, page)->vt,
		MotionNotify, ev))
		;
	    XQueryPointer(r->Xdisplay, PVTS(r, page)->vt,
		&unused_root, &unused_child,
		&unused_root_x, &unused_root_y,
		&(ev->xbutton.x), &(ev->xbutton.y),
		&unused_mask);
#ifdef MOUSE_THRESHOLD
	    /* deal with a `jumpy' mouse */
	    if ((ev->xmotion.time - r->h->MEvent.time) > MOUSE_THRESHOLD)
	    {
#endif
		rxvt_selection_extend(r, page,
		    (ev->xbutton.x), (ev->xbutton.y),
		    (ev->xbutton.state & Button3Mask) ? 2 : 0);
#ifdef SELECTION_SCROLLING
		if (
			ev->xbutton.y<r->TermWin.int_bwidth
			|| Pixel2Row(ev->xbutton.y)>(r->TermWin.nrow-1)
		   )
		{
		    int dist;

		    r->h->pending_scroll_selection=1;

		    /*
		    ** don't clobber the current delay if we are
		    ** already in the middle of scrolling.
		    */
		if (r->h->scroll_selection_delay<=0)
		    r->h->scroll_selection_delay = SCROLLBAR_CONTINUOUS_DELAY;

		    /*
		    ** save the event params so we can highlight
		    ** the selection in the pending-scroll loop
		    */
		r->h->selection_save_x = ev->xbutton.x;
		r->h->selection_save_y = ev->xbutton.y;
		r->h->selection_save_state = (ev->xbutton.state & Button3Mask) ? 2 : 0;

		    /* calc number of lines to scroll */
		    if (ev->xbutton.y<r->TermWin.int_bwidth)
		    {
			r->h->scroll_selection_dir = UP;
			dist = r->TermWin.int_bwidth - ev->xbutton.y;
		    }
		    else
		    {
			r->h->scroll_selection_dir = DN;
			dist = ev->xbutton.y -
			(r->TermWin.int_bwidth + VT_HEIGHT(r));
		    }
		    r->h->scroll_selection_lines=(Pixel2Height(dist)/
			SELECTION_SCROLL_LINE_SPEEDUP)+1;
		    MIN_IT(r->h->scroll_selection_lines,
			SELECTION_SCROLL_MAX_LINES);
		}
		else
		{
		    /*
		    ** we are within the text window, so we
		    ** shouldn't be scrolling
		    */
		    r->h->pending_scroll_selection = 0;
		}
#endif
#ifdef MOUSE_THRESHOLD
	    }
#endif
	}
    }
#ifdef HAVE_SCROLLBARS
    else if (
		rxvt_is_scrollbar_win (r, ev->xany.window)
		&& scrollbar_isMotion()
	    )
    {
	while (
		XCheckTypedWindowEvent(r->Xdisplay,
		    r->scrollBar.win, MotionNotify, ev)
	      )
	    ;

	XQueryPointer(r->Xdisplay, r->scrollBar.win,
	    &unused_root, &unused_child,
	    &unused_root_x, &unused_root_y,
	    &(ev->xbutton.x), &(ev->xbutton.y),
	    &unused_mask);
	rxvt_scr_move_to(r, page,
	    scrollbar_position(ev->xbutton.y) - r->h->csrO,
	    scrollbar_size());

	rxvt_scr_refresh(r, page, r->h->refresh_type & ~CLIPPED_REFRESH);
	rxvt_scrollbar_update(r, 1);
    }
#endif
}