void
xftDrawACSString ( Display *dpy, Drawable d, GC gc,
	void (*xftdraw_string)(),
	XftDraw *draw, const XftColor *color, XftFont *pub,
	int x, int y, const unsigned char *str, int len)
{
    const unsigned char *t = str;
    int chars;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "Drawing %d(%d) %sACS characters.", len, STRLEN( str), ( xftdraw_string == XftDrawString8) ? "Utf8 " : ""));

    while(len > 0)
    {
	/*
	 * Pass all non graphic chars to xftdraw_string.
	 */
	for( chars=0; *t >= 32 && chars < len; chars++, t++);
	if( chars)
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, " [%d chars]", chars));
	    xftdraw_string( draw, color, pub, x, y, str, chars);

	    x += chars * pub->max_advance_width;
	    str = t;
	    len -= chars;
	}

	/*
	 * Draw all ACS graphics chars by hand.
	 */
	for( chars=0; *t < 32 && chars < len; chars++, t++);
	if( chars)
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, " (%d glyphs)", chars));
	    xftDrawACSChars( dpy, d, gc, draw, color, pub, x, y, str, chars);

	    x += chars * pub->max_advance_width;
	    str = t;
	    len -= chars;
	}
    }
#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_XFTACS, "\n"));
#endif
}