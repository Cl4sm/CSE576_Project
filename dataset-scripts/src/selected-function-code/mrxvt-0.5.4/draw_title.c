static void
draw_title (rxvt_t* r, int x, int y, int tnum, Region region)
{
    Region	clipRegion;
    char	str[MAX_DISPLAY_TAB_TXT + 1];

#ifdef MULTICHAR_SET
    char	buf[MAX_TAB_TXT + 1];
    const char*	sptr;
    const char*	ptr;
    int		multichar;
    int		len;
#endif

    UNSET_REGION( clipRegion );

    /*
     * Adjust y offset, and make sure output is restricted to the current tab
     * title.
     */
#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft) && (NULL != r->tabBar.xftwin))
    {
	if( r->TermWin.xftpfont )
	{
	    /*
	     * If we use pfont to draw tab titles, the we dont' know how many
	     * characters will fit on the title. So we should clip the output
	     * correctly.
	     */
	    XRectangle rect;

	    rect.x = x;
	    rect.y = y - r->TermWin.pheight;
	    rect.width = PVTS(r, tnum)->tab_width - 2*TXT_XOFF;
	    rect.height = r->TermWin.pheight;

	    clipRegion = XCreateRegion();
	    XUnionRectWithRegion( &rect, clipRegion, clipRegion);

	    if (IS_REGION(region))
		XIntersectRegion( clipRegion, region, clipRegion);

	    XftDrawSetClip( r->tabBar.xftwin, clipRegion);

	    y -= r->TermWin.xftpfont->descent;
	}
	else y -= r->TermWin.xftfont->descent;
    }
    else
#endif /* XFT_SUPPORT */
	y -= r->TermWin.font->descent;

    /*
     * Get the title into str. Under Xft, we use the format specified by
     * title_format.
     */
#ifdef XFT_SUPPORT
    if(
	  NOTSET_OPTION( r, Opt_xft )			||
	  IS_NULL( r->TermWin.xftpfont )		||
	  IS_NULL( PVTS(r, tnum)->title_format )   	||
	  rxvt_percent_interpolate( r, tnum,
		PVTS(r, tnum)->title_format,
		STRLEN( PVTS(r, tnum)->title_format ),
		str, r->TermWin.maxTabWidth ) <= 1
      )
#endif /* XFT_SUPPORT */
    {
	/*
	 * If % interpolation was not possible, or returned a 1 byte long
	 * string, then just copy the title over.
	 */
	STRNCPY( str, PVTS(r,tnum)->tab_title , r->TermWin.maxTabWidth );
	str[r->TermWin.maxTabWidth] = '\0';
    }


    /*
     * Draw the string (different code for multichar / non-multichar).
     */
#ifdef MULTICHAR_SET
    sptr = ptr = str;
    multichar = (*ptr & 0x80);
    while (*ptr)
    {
	if (multichar && (*ptr & 0x80))	    /* multichar */
	    ptr ++;
	else if (!multichar && !(*ptr & 0x80))	/* single char */
	    ptr ++;
	else
	{
	    len = ptr - sptr;
	    /* adjust bytes, must be 2x for multichar */
	    if (multichar && (len % 2) != 0)
	    {
		len ++; ptr ++;
		/* continue to next byte, we shouldn't stop here */
		continue;
	    }
	    assert (len <= MAX_TAB_TXT);

	    memcpy (buf, sptr, len);
	    buf[len] = (char) 0;
	    x += draw_string (r, clipRegion,
		    x, y, buf, len, multichar, tnum == ATAB(r));

	    /* adjust start position */
	    /* x += Width2Pixel(len); */
	    /*
#ifdef XFT_SUPPORT
	    if (ISSET_OPTION(r, Opt_xft) && r->tabBar.xftwin)
	    {
		x += Width2Pixel(len);
	    }
	    else
#endif
	    {
		if (multichar)
		    x += XTextWidth (r->TermWin.mfont, buf, len/2);
		else
		    x += XTextWidth (r->TermWin.font, buf, len);
	    }
	    */

	    /* ok, now the next sub-string */
	    sptr = ptr;
	    multichar = (*ptr & 0x80);
	    if ((char) 0 == *ptr)
		break;	/* in case ptr is increased at line 356 */
	    ptr ++;
	}
    }

    /* last sub-string */
    len = ptr - sptr;
    if (0 != len)	/* in case last sub-string is empty */
    {
	memcpy (buf, sptr, len);
	buf[len] = (char) 0;
	draw_string (r, clipRegion,
		x, y, buf, len, multichar, tnum == ATAB(r));
    }

#else	/* MULTICHAR_SET */
    draw_string (r, clipRegion,
	    x, y, str, STRLEN(str), False, tnum == ATAB(r));
#endif	/* MULTICHAR_SET */

    /*
     * Restore clipping of the xftdrawable / gc.
     */
    if (IS_REGION(clipRegion))
    {
	XDestroyRegion( clipRegion);

	if (NOT_REGION(region))
	    XSetClipMask( r->Xdisplay, r->tabBar.gc, None);
	else
	    XSetRegion( r->Xdisplay, r->tabBar.gc, region);
#ifdef XFT_SUPPORT
	if (r->tabBar.xftwin)
	    XftDrawSetClip( r->tabBar.xftwin, region);
#endif
    }
}