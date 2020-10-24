CARD32
rxvt_get_desktop( rxvt_t* r )
{
    Atom    ret_type;
    int	    format;
    unsigned long   nitems;
    unsigned long   bytes_after;
    unsigned char   *prop;
    CARD32  desktop;

    if (NOT_ATOM(r->h->xa[XA_NET_WM_DESKTOP]))
	return 0;

    if(
	  XGetWindowProperty( r->Xdisplay, r->TermWin.parent,
	      r->h->xa[XA_NET_WM_DESKTOP], 0L, LONG_MAX, False, XA_CARDINAL,
	      &ret_type, &format, &nitems, &bytes_after, &prop)
	    != Success
      )
	return 0;

    if( ret_type == XA_CARDINAL && format == 32 )
    {
	desktop = ((CARD32*) prop)[0];
	if (desktop < 0 || desktop > 64)
	    desktop = 0;
    }
    else
	desktop = 0;

    XFree( prop);

    rxvt_dbgmsg(( DBG_DEBUG, DBG_INIT, "Desktop: %lu\n", desktop ));
    return desktop;
}