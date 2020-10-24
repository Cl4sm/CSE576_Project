void
rxvt_process_configurenotify (rxvt_t* r, XConfigureEvent* ev)
{
    unsigned int    height, width;

    if (ev->window != r->TermWin.parent)
	return;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "ConfigureNotify event\n"));
    do		    /* Wrap lots of configures into one */
      {
	width = ev->width;
	height = ev->height;
      }
    while ( XCheckTypedWindowEvent(r->Xdisplay, ev->window,
		    ConfigureNotify, (XEvent*) ev));

    /*
     * Remember the position of the window. Make sure that we remember the
     * position relative to the root window (for root transparency). Notice this
     * position will be offset by the Window manager.
     */
    XTranslateCoordinates(r->Xdisplay, r->TermWin.parent, XROOT,
	0, 0, &r->szHint.x, &r->szHint.y, &ev->window);

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Geometry: %ux%u+%d+%d\n", width, height, r->szHint.x, r->szHint.y));

    if (
	    r->h->want_resize
	    || r->szHint.width != width || r->szHint.height != height
       )
    {

	rxvt_resize_on_configure (r, width, height);
    }
#ifdef DEBUG
    else
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Size: Not resizing\n"));
    }
#endif

    if( (r->h->want_resize & ~FORCE_REFRESH) )
    {
	r->h->want_resize = 0;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Forcing refresh (from want_resize)\n"));

#if defined(HAVE_LIBXRENDER) && (defined(BACKGROUND_IMAGE) || defined(TRANSPARENT))
	/*
	 * Background images tinted with XRender need to be refreshed on hide /
	 * show of sub windows.
	 */
	rxvt_refresh_bg_image( r, ATAB(r), False );
#else
	/*
	 * Otherwise we should refresh the screen. A lazy refresh is enough, as
	 * we will recieve expose events shortly.
	 */
	/* rxvt_src_clear required? */
	rxvt_scr_touch( r, ATAB(r), False);
#endif
    }

#ifdef TRANSPARENT
    /*
     * Check to see if the previous position we grabbed the background is
     * different from the current window position. If yes, then update
     * everything.
     */
    if (ISSET_OPTION(r, Opt_transparent))
    {
	if(
		!r->h->bgGrabbed
		|| r->h->prevPos.x != r->szHint.x
		|| r->h->prevPos.y != r->szHint.y
		|| r->h->prevPos.width != width
		|| r->h->prevPos.height != height
	  )
	{
	    /*
	     * Rather than refresh our background pixmap now (which is laggy as
	     * hell), generate a timeout. So if the user is dragging this
	     * window, we won't refresh our background unless he stops for a
	     * certain ammount of time.
	     */
	    if( !r->h->bgRefreshInterval)
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "refreshing parents...\n"));
		if( rxvt_check_our_parents(r) )
		{
		    r->h->want_resize = 0;
		    r->h->want_full_refresh = 1;
		}
	    }
	    else
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Setting timeout for parent refresh.\n"));
		gettimeofday( &r->h->lastCNotify, NULL);
	    }
	}
    }
#endif
}