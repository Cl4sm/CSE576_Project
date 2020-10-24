void
rxvt_process_x_event(rxvt_t* r, XEvent *ev)
{
    int		    page = ATAB(r);
    int		    i, want_timeout = 0;
    struct timeval  tp;
    struct rxvt_hidden *h = r->h;
#ifdef DEBUG_X
    const char *const eventnames[] =
    {		    /* mason - this matches my system */
	"",
	"",
	"KeyPress",
	"KeyRelease",
	"ButtonPress",
	"ButtonRelease",
	"MotionNotify",
	"EnterNotify",
	"LeaveNotify",
	"FocusIn",
	"FocusOut",
	"KeymapNotify",
	"Expose",
	"GraphicsExpose",
	"NoExpose",
	"VisibilityNotify",
	"CreateNotify",
	"DestroyNotify",
	"UnmapNotify",
	"MapNotify",
	"MapRequest",
	"ReparentNotify",
	"ConfigureNotify",
	"ConfigureRequest",
	"GravityNotify",
	"ResizeRequest",
	"CirculateNotify",
	"CirculateRequest",
	"PropertyNotify",
	"SelectionClear",
	"SelectionRequest",
	"SelectionNotify",
	"ColormapNotify",
	"ClientMessage",
	"MappingNotify"
    };
    struct tm	  *ltt;
#endif

    /*
     * check if we need to get the time for any timeouts
     */

    for (i = NUM_TIMEOUTS; i--; )
	if (h->timeout[i].tv_sec)
	{
	    want_timeout = 1;
	    break;
	}

#ifndef DEBUG_X
    if (want_timeout)
#endif
	(void)gettimeofday(&tp, NULL);

#ifdef DEBUG_X
    ltt = localtime(&(tp.tv_sec));
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Event: %-16s %-7s %08lx (%4d-%02d-%02d %02d:%02d:%02d.%.6ld) %s %lu\n", eventnames[ev->type], (ev->xany.window == r->TermWin.parent ? "parent" : (ev->xany.window == PVTS(r, page)->vt ? "vt" : (ev->xany.window == r->scrollBar.win ? "scroll" : (ev->xany.window == r->menuBar.win ? "menubar" : "UNKNOWN")))), (ev->xany.window == r->TermWin.parent ? r->TermWin.parent : (ev->xany.window == PVTS(r, page)->vt ? PVTS(r, page)->vt : (ev->xany.window == r->scrollBar.win ? r->scrollBar.win : (ev->xany.window == r->menuBar.win ? r->menuBar.win : 0)))), ltt->tm_year + 1900, ltt->tm_mon + 1, ltt->tm_mday, ltt->tm_hour, ltt->tm_min, ltt->tm_sec, tp.tv_usec, ev->xany.send_event ? "S" : " ", ev->xany.serial));
#endif

    /* X event timeouts */
    if (want_timeout)
    {
	for (i = NUM_TIMEOUTS; i--; )
	{
	    if (
		    h->timeout[i].tv_sec == 0		||
		    tp.tv_sec < h->timeout[i].tv_sec	||
		    (
		      tp.tv_sec == h->timeout[i].tv_sec	    &&
		      tp.tv_usec < h->timeout[i].tv_usec
		    )
	       )
		continue;

	    /* We timed out on the "i"th timeout */
	    h->timeout[i].tv_sec = 0;
	    switch(i)
	    {
		case TIMEOUT_INCR:
		    rxvt_msg (DBG_ERROR, DBG_COMMAND,
			"data loss: timeout on INCR selection paste");
		    h->selection_wait = Sel_none;
		    break;

		default:
		    break;
	    }
	}
    }


    switch (ev->type)
    {
	case KeyPress:
	    rxvt_process_keypress (r, (XKeyEvent *) ev);
	    break;

#if defined(MOUSE_WHEEL) && defined(MOUSE_SLIP_WHEELING)
	case KeyRelease:
	    rxvt_process_keyrelease (r, (XKeyEvent*) ev);
	    break;
#endif

	case ButtonPress:
	    rxvt_process_buttonpress(r, page, (XButtonEvent *) ev);
	    break;

	case ButtonRelease:
	    rxvt_process_buttonrelease(r, page, (XButtonEvent *) ev);
	    break;

	case ClientMessage:
	    rxvt_process_clientmessage(r, (XClientMessageEvent*) ev);
	    break;

	case MappingNotify:
	    XRefreshKeyboardMapping(&(ev->xmapping));
	    break;

	/*
	 * XXX: this is not the _current_ arrangement
	 * Here's my conclusion:
	 * If the window is completely unobscured, use bitblt's
	 * to scroll. Even then, they're only used when doing partial
	 * screen scrolling. When partially obscured, we have to fill
	 * in the GraphicsExpose parts, which means that after each refresh,
	 * we need to wait for the graphics expose or Noexpose events,
	 * which ought to make things real slow!
	 */
	case VisibilityNotify:
	    rxvt_process_visibilitynotify (r, (XVisibilityEvent*) ev);
	    break;

#ifdef MONITOR_ENTER_LEAVE
	case EnterNotify:
	    rxvt_process_enter (r, (XCrossingEvent*) ev);
	    break;

	case LeaveNotify:
	    rxvt_process_leave (r, (XCrossingEvent*) ev);
	    break;
#endif	/* MONITOR_ENTER_LEAVE */

	case FocusIn:
	    rxvt_process_focusin (r, (XFocusChangeEvent*) ev);
	    break;

	case FocusOut:
	    rxvt_process_focusout (r, (XFocusChangeEvent*) ev);
	    break;

	case ConfigureNotify:
	    rxvt_process_configurenotify (r, (XConfigureEvent*) ev);
	    break;

	case SelectionClear:
	    rxvt_process_selectionclear(r, page);
	    break;

	case SelectionNotify:
	    rxvt_process_selectionnotify (r, (XSelectionEvent*) ev);
	    break;

	case SelectionRequest:
	    rxvt_process_selectionrequest (r, page, (XSelectionRequestEvent*) ev);
	    break;

	case UnmapNotify:
	    r->TermWin.mapped = 0;
	    break;

	case MapNotify:
	    r->TermWin.mapped = 1;
	    break;

	case PropertyNotify:
	    rxvt_process_propertynotify (r, ev);
	    break;

	case ReparentNotify:
	    rxvt_process_reparentnotify (r, ev);
	    break;

	case GraphicsExpose:
	case Expose:
	    rxvt_process_expose (r, ev);
	    break;

	case MotionNotify:
	    rxvt_process_motionnotify (r, ev);
	    break;
    }
}