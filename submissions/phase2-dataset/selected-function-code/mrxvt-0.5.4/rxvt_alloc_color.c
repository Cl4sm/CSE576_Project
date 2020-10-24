int
rxvt_alloc_color( rxvt_t* r, XColor *screen_in_out, const char *colour )
{
    return XAllocColor( r->Xdisplay, XCMAP, screen_in_out );

#if 0 /* 2006-05-27 gi1242: Really bad code. */
    int		    res;

    if( (res = XAllocColor(r->Xdisplay, XCMAP, screen_in_out)) )
	return res;

    /* try again with closest match */
    /*
     * XXX 2006-05-25 gi1242: This is really inefficient. There must be a better
     * way!
     */
    if (XDEPTH >= 4 && XDEPTH <= 8)
    {
	int		i, numcol;
	int		best_pixel = 0;
	unsigned long   best_diff, diff;
	XColor		*colors;

#define rSQR(x)	    ((x)*(x))

	best_diff = 0;
	numcol = 0x01 << XDEPTH;
	if( (colors = rxvt_malloc(numcol * sizeof(XColor))) )
	{
	    for (i = 0; i < numcol; i++)
	    colors[i].pixel = i;

	    XQueryColors(r->Xdisplay, XCMAP, colors, numcol);
	    for (i = 0; i < numcol; i++)
	    {
		diff = rSQR(screen_in_out->red - colors[i].red)
		    + rSQR(screen_in_out->green - colors[i].green)
		    + rSQR(screen_in_out->blue - colors[i].blue);
		if (i == 0 || diff < best_diff)
		{
		    best_pixel = colors[i].pixel;
		    best_diff = diff;
		}
	    }
	    *screen_in_out = colors[best_pixel];
	    rxvt_free(colors);

	    res = XAllocColor(r->Xdisplay, XCMAP, screen_in_out);
	}
    }

    if (res == 0)
	rxvt_msg (DBG_ERROR, DBG_MAIN, "can't allocate color: %s", colour);

    return res;
#endif
}