void
rxvt_process_propertynotify (rxvt_t* r, XEvent* ev)
{
#ifdef TRANSPARENT
    int wantRefresh = 0;    /* Want transparency refresh */
#endif

#ifdef DEBUG
    char *name;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Property notify event:"));
#endif

    assert( ev->type == PropertyNotify );
    /*
     * Wrap all property notify events in one loop. This is *required* so that
     * we don't run the transparency updates too often (or worse still, with a
     * bad pixmap).
     */
    do
      {
#ifdef DEBUG
	name = XGetAtomName( r->Xdisplay, ev->xproperty.atom);
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s(%08lx) ", name, ev->xproperty.atom ));
	XFree( name);
#endif

	if (
		ev->xproperty.atom == r->h->xa[XA_VT_SELECTION]
		&& ev->xproperty.state == PropertyNewValue
	   )
	{
	    rxvt_selection_property(r, ev->xproperty.window, ev->xproperty.atom);
	}

#ifdef TRANSPARENT
	else if (IS_ATOM(r->h->xa[XA_XROOTPMAPID]) &&
		 IS_ATOM(r->h->xa[XA_XSETROOTID]) &&
		 (ev->xproperty.atom == r->h->xa[XA_XROOTPMAPID] ||
		  ev->xproperty.atom == r->h->xa[XA_XSETROOTID]
		 )
		)
	{
	    /*
	     * If user used some Esetroot compatible prog to set the root bg,
	     * use the property to determine the pixmap.  We use it later on.
	     */
	    if( !wantRefresh)
	    {
		/*
		 * A few programs (fvwm-root for instance) use the XSETROOTID
		 * property to indicate changes in transparency (they set it to
		 * none when the background is a solid color). So we have to
		 * listen for both properties.
		 *
		 * fvwm-root (and some other badly behaved programs) do NOT grab
		 * the server when setting the above atoms. Thus while our event
		 * queue might be empty now, we might get a PropertyNotify event
		 * setting one of the above atoms shortly. Thus we introduce a
		 * delay here (eliminates flicker). Notice that this will NOT
		 * introduce a delay when listening for other properties.
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "(sleeping ... "));
#ifdef HAVE_NANOSLEEP
		struct timespec rqt;
		rqt.tv_sec = 0;
		rqt.tv_nsec = 10000000; /* 10 ms */
		nanosleep(&rqt, NULL);
#else
		sleep( 1);
#endif
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "done) "));
	    }
	    wantRefresh = 1;
	}

#endif /* TRANSPARENT */
      }
    while( XCheckTypedEvent( r->Xdisplay, PropertyNotify, ev) );

#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "\n"));
#endif
#ifdef TRANSPARENT
    if( wantRefresh)
    {
	/*
	 * Refresh bg vars EVEN if we're not transparent. Thus if we toggle
	 * transparency later on we don't have to worry about it
	 */
	refreshRootBGVars( r );
	if (ISSET_OPTION(r, Opt_transparent))
	{
	    /*
	     * Let rxvt_check_our_parents worry about refreshRootBGVars failing.
	     */
	    rxvt_check_our_parents(r);
	    r->h->want_full_refresh = 1;
	}
    }
#endif	    /* TRANSPARENT */
}