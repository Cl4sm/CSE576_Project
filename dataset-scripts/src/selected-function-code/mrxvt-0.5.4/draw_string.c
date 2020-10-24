static int
draw_string (rxvt_t* r, Region clipRegion,
	int x, int y, char* str, int len,
	__attribute__((unused)) int multichar, int active)
{
#ifdef XFT_SUPPORT
    XGlyphInfo	ginfo;
#endif

#ifdef MULTICHAR_SET
    if (multichar)
    {
	/*
	 * Draw the multichar string
	 */
# ifdef XFT_SUPPORT

	if (ISSET_OPTION(r, Opt_xft) && (NULL != r->tabBar.xftwin))
	{
#  ifdef HAVE_ICONV_H
	    if (
		    ENC_NOENC != r->encoding_method
		    && (iconv_t) -1 != r->TermWin.xfticonv
	       )
	    {
		char		buf[1024];
		int		plen = 1023;
		char*		pstr = buf;
		int		olen = len;
		char*		ostr = str;

		/* convert to UTF-8 */
		iconv (r->TermWin.xfticonv, (char**) &ostr,
		    (size_t*) &olen, &pstr, (size_t*) &plen);
		*pstr = (char) 0;   /* set end of string */

		rxvt_draw_string_xft (r, r->tabBar.win, r->tabBar.gc,
			clipRegion, RS_None, 
			active ? USE_BOLD_PFONT : USE_PFONT,
			r->tabBar.xftwin,
			active ? &(r->tabBar.xftfg) : &(r->tabBar.xftifg),
			x, y, buf, len, XftDrawStringUtf8);
		if( r->TermWin.xftpfont )
		{
		    XftTextExtentsUtf8( r->Xdisplay, r->TermWin.xftpfont,
			    (unsigned char*) buf, pstr - buf, &ginfo);
		    return ginfo.width;
		}
		else return Width2Pixel( pstr - buf );
	    }
	    else
#  endif
	    {
		rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "XFT non-iconv tab title\n"));
		rxvt_draw_string_xft (r, r->tabBar.win, r->tabBar.gc,
			clipRegion, RS_None,
			active ? USE_BOLD_PFONT : USE_PFONT,
			r->tabBar.xftwin,
			active ? &(r->tabBar.xftfg) : &(r->tabBar.xftifg),
			x, y, str, len, XftDrawString8);

		if( r->TermWin.xftpfont )
		{
		    XftTextExtents8( r->Xdisplay, r->TermWin.xftpfont,
			    (unsigned char*) str, len, &ginfo);
		    return ginfo.width;
		}
		else return Width2Pixel( len );
	    }
	}
	else
# endif	/* XFT_SUPPORT */
	{
	    if (ENC_NOENC != r->encoding_method)
	    {
		XSetFont (r->Xdisplay, r->tabBar.gc, r->TermWin.mfont->fid);
		r->h->multichar_decode ( (unsigned char*) str, len);
		rxvt_draw_string_x11 (r, r->tabBar.win, r->tabBar.gc,
			clipRegion, x, y, str, len/2, XDrawString16);
	    }
	    else
	    {
		XSetFont (r->Xdisplay, r->tabBar.gc, r->TermWin.font->fid);
		rxvt_draw_string_x11 (r, r->tabBar.win, r->tabBar.gc,
			clipRegion, x, y, str, len, XDrawString);
	    }
	    return Width2Pixel( len );
	}
    } /* if (multichar) */

    else
#endif /* MULTICHAR_SET */
    {
	/*
	 * Draw the non-multichar string
	 */
# ifdef XFT_SUPPORT
	if (ISSET_OPTION(r, Opt_xft) && (NULL != r->tabBar.xftwin))
	{
	    rxvt_draw_string_xft (r, r->tabBar.win, r->tabBar.gc,
		    clipRegion, RS_None,
		    active ? USE_BOLD_PFONT : USE_PFONT,
		    r->tabBar.xftwin,
		    active ? &(r->tabBar.xftfg) : &(r->tabBar.xftifg),
		    x, y, str, len, XftDrawString8);

	    if( r->TermWin.xftpfont )
	    {
		XftTextExtents8( r->Xdisplay, r->TermWin.xftpfont,
			(unsigned char*) str, len, &ginfo);
		return ginfo.width;
	    }
	    else return Width2Pixel( len );
	}
	else
# endif	/* XFT_SUPPORT */
	{
	    XSetFont (r->Xdisplay, r->tabBar.gc, r->TermWin.font->fid);
	    rxvt_draw_string_x11 (r, r->tabBar.win, r->tabBar.gc,
		    clipRegion, x, y, str, len, XDrawString);
	    return Width2Pixel( len );
	}
    }
}