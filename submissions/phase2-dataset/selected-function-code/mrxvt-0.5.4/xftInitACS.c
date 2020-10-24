void
xftInitACS( Display *dpy, Drawable d, unsigned depth)
{
    int (*oldXerrorHandler)( Display *, XErrorEvent *);

    rxvt_dbgmsg ((DBG_DEBUG, DBG_XFTACS, "Initing sPmap\n"));

    if(d == 0)		d = DefaultRootWindow( dpy);
    if(depth == 0)	depth=DefaultDepth( dpy, DefaultScreen( dpy));

#ifdef DEBUG
    if( sPmap != 0)
	rxvt_dbgmsg ((DBG_DEBUG, DBG_XFTACS, "sPmap not null in xftInitACS"));
#endif

    acsGc = XCreateGC( dpy, d, 0, NULL);

    oldXerrorHandler = XSetErrorHandler( (XErrorHandler) acsXErrorHandler);
    sPmap = XCreatePixmap( dpy, d, 2, 2, depth);
    XSetErrorHandler( oldXerrorHandler);

    XSetTile( dpy, acsGc, sPmap);
}