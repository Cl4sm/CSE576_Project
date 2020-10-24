void
rxvt_process_expose (rxvt_t* r, XEvent* ev)
{
    int		page = ATAB(r);
    Window	win = (ev->type == Expose ?
			ev->xexpose.window : ev->xgraphicsexpose.drawable );

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Got %s event.\n", ev->type == Expose ? "Expose" : "GraphicsExpose"));
    if (win == PVTS(r, page)->vt)
    {
	/*
	 * We need to be clever about exposes (especially under oober slow Xft).
	 * We group exposes, and clip to the appropriate locations.
	 */

	if (NOT_REGION(r->h->refreshRegion))
	{
	    r->h->refreshRegion = XCreateRegion();
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Created clip region %p\n", r->h->refreshRegion));
	    /* Will be freed when rxvt_scr_refresh completes */
	}

	do
	  {
	    XRectangle rect;

	    if( ev->type == Expose)
	    {
		rect.x	    = ev->xexpose.x;
		rect.y	    = ev->xexpose.y;
		rect.width  = ev->xexpose.width;
		rect.height = ev->xexpose.height;
	    }
	    else
	    {
		rect.x	    = ev->xgraphicsexpose.x;
		rect.y	    = ev->xgraphicsexpose.y;
		rect.width  = ev->xgraphicsexpose.width;
		rect.height = ev->xgraphicsexpose.height;
	    }

	    /*
	     * BUG# 1473987: We sometimes receive expose events when the window
	     * contents are not completely cleared.
	     *
	     * If this does not fix the bug, then we will have to issue an
	     * XClearArea() regardless of weather send_event is true or false.
	     */
#if 0
	    /*
	     * 2006-05-20 gi1242: This did not fix the bug, so we comment out
	     * the test for send_event.
	     */
	    if( ev->xexpose.send_event )
#endif
		XClearArea( r->Xdisplay, win, rect.x, rect.y,
			rect.width, rect.height, False );

	    XUnionRectWithRegion( &rect, r->h->refreshRegion,
		    r->h->refreshRegion);
	    rxvt_scr_expose(r, page,
		    rect.x, rect.y, rect.width, rect.height,
		    False); /* Don't generate a screen refresh */


	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Adding [%ux%u+%d+%d] to expose region\n", rect.width, rect.height, rect.x, rect.y));
	  }
	while(
		XCheckTypedWindowEvent( r->Xdisplay, win, Expose, ev)
		|| XCheckTypedWindowEvent( r->Xdisplay, win, GraphicsExpose, ev)
	     );

	r->h->refresh_type |= CLIPPED_REFRESH;
	r->h->want_clip_refresh = 1;
    }
    else
    {
	XEvent	    unused_xevent;

#ifdef DEBUG
	/* Debug message */
	if (rxvt_is_tabbar_win (r, win))
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Expose event on tabbar\n"));
	}
# ifdef HAVE_SCROLLBARS
	else if (rxvt_is_scrollbar_win (r, win))
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Expose event on scrollbar\n"));
	}
# endif
# ifdef HAVE_MENUBAR
	else if (rxvt_is_menubar_win (r, win))
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Expose event on menubar\n"));
	}
# endif
#endif	/* DEBUG */

	if (rxvt_is_tabbar_win(r, win) && rxvt_tabbar_visible (r))
	{
	    rxvt_tabbar_expose(r, ev);
	    return;
	}

	/* fetch all expose events if possible */
	while(	XCheckTypedWindowEvent(r->Xdisplay, win,
		    Expose, &unused_xevent) ||
		XCheckTypedWindowEvent( r->Xdisplay, win,
		    GraphicsExpose, &unused_xevent)
	     );

#ifdef HAVE_SCROLLBARS
	if ( rxvt_is_scrollbar_win (r, win) && rxvt_scrollbar_visible (r))
	{
	    scrollbar_setIdle();
	    rxvt_scrollbar_update(r, 0);
	    return;
	}
#endif
#ifdef HAVE_MENUBAR
	if (rxvt_is_menubar_win(r, win) && rxvt_menubar_visible (r))
	{
	    rxvt_menubar_expose(r);
	    return;
	}
#endif
	/* Not reached */
    }
}