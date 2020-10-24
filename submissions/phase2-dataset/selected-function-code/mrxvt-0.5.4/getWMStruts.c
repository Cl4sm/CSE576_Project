Bool getWMStruts( Display *dpy, Window w,
	CARD32 *left, CARD32 *right, CARD32 *top, CARD32 *bottom)
{

    Atom atom;
    static const int natoms = 2;
    static const char *atomName[2] = {
	"_KDE_NET_WM_FRAME_STRUT",
	"_NET_FRAME_EXTENTS"};
    int i;

    Atom	    type;
    int		    format;
    unsigned long   nitems, bytes_after;

    unsigned char  *prop; /* left, right, top and bottom borders */

    /*
     * Initialize return values to 0, just incase we can't read the window
     * property.
     */
    *left = *right = *top = *bottom = 0;

    /* Now try and read the property */
    for( i=0; i < natoms; i++)
    {
	atom = XInternAtom( dpy, atomName[i], False);
	if (NOT_ATOM(atom))
	    continue;

	if( XGetWindowProperty( dpy, w, atom,
		0,		/* offset */
		4,		/* length */
		False,		/* Delete? */
		XA_CARDINAL,	/* Type */
		&type, &format, &nitems, &bytes_after, /* return values */
		&prop) != Success )
	    continue;

	if(
	    type == XA_CARDINAL && bytes_after == 0 &&
	    format == 32 && nitems == 4
	  )
	{
	    CARD32  *struts = (CARD32*) prop;

	    *left   = struts[0];
	    *right  = struts[1];
	    *top    = struts[2];
	    *bottom = struts[3];

	    rxvt_dbgmsg((DBG_DEBUG, DBG_COMMAND,
			"%s: %lu, %lu, %lu, %lu (format=%d, nitems=%lu)\n",
			atomName[i], *left, *right, *top, *bottom,
			format, nitems ));
	    /*
	     * Don't check future atomName's
	     */
	    i = natoms;
	}

	XFree( prop);
    }
    return i == natoms + 1;
}