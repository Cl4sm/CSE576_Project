*/
/* INTPROTO */
void
rxvt_scr_draw_string (rxvt_t* r, int page,
	int x, int y, char* str, int len, int drawfunc,
	uint16_t fore, uint16_t back,
	__attribute__((unused)) rend_t rend, Region refreshRegion)
{
#ifdef XFT_SUPPORT
    int	    fillback = 0;
    int	    adjust;
    void    (*xftdraw_string) () = NULL;

    switch (drawfunc)
    {
	case	XFT_DRAW_IMAGE_STRING_8:
	    fillback = 1;
	case	XFT_DRAW_STRING_8:
	    xftdraw_string = XftDrawString8; break;

	case	XFT_DRAW_IMAGE_STRING_16:
	    fillback = 1;
	case	XFT_DRAW_STRING_16:
	    xftdraw_string = XftDrawString16; break;
    }

    /*
     * adjust is a variable that records whether each character of the string is
     * 8 bits or 16 bits
     */
    adjust = (XftDrawString8 == xftdraw_string) ? 0 : 1;

    if (ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt && xftdraw_string)
    {
	register int	loop;	    /* loop iteration number */
	register int	loopitem;   /* each iteration increasing # */
	register int	i;
	/*
	** xft_draw_string_xft should call these two parameters
	*/
	register char*	pstr;	    /* string to print */
	register int	plen;	    /* string length */
	char*		newstr;
#ifdef MULTICHAR_SET
#  ifdef HAVE_ICONV_H
	char		pbuf[1024]; /* buffer to save UTF-8 string */
#  endif
#endif


	/*
	 * Xft does not support XDrawImageString, so we need to clear the
	 * background of text by ourselves.
	 */
	if (fillback)
	    XftDrawRect( PVTS(r, page)->xftvt, &(r->xftColors[back]),
		    x, y, Width2Pixel(len * (1 + adjust)), Height2Pixel(1));

	/* We use TermWin.xftfont->ascent here */
	y += r->TermWin.xftfont->ascent;

	/*
	 * Xft does not support XftDrawString16, so we need to convert the
	 * string to UTF-8. Here we reencode the string before conversion
	 */
# ifdef MULTICHAR_SET
#  ifdef HAVE_ICONV_H
	if (adjust && (iconv_t) -1 != r->TermWin.xfticonv)
	{
	    register int    j, newlen = (len << 1);
	    switch (r->encoding_method)
	    {
		case ENC_EUCJ:
		case ENC_GB:
		    for (j = 0; j < newlen; j ++)
			str[j] |= 0x80;
		    break;
		case ENC_GBK:	/* need to do nothing */
		case ENC_BIG5:	/* need to do nothing */
		default:
		    break;
	    }
	    /* we will use utf8 routine to draw string */
	    xftdraw_string = XftDrawStringUtf8;
	}
#  endif
# endif	/* MULTICHAR_SET */


	/*
	** If the font is monospace, we print the entire string once,
	** otherwise, print the characters one by one
	*/
	if (r->TermWin.xftmono)
	{
	    /* print string once for mono font */
	    loop = 1;
	    loopitem = len;
	    /*
	    ** If XftDrawString8 == xftdraw_string
	    **     string length == character number
	    ** If XftDrawString16 == xftdraw_string
	    **     string length == 2 * character number, but we do
	    ** not need to multiply loopitem by 2 because  the
	    ** XftDrawString16 takes character numbers.
	    **
	    ** If XftDrawStringUtf8 == xftdraw_string
	    **     string length == 2 * character number, but we need
	    ** to multiply loopitem by 2 because iconv need string
	    ** length as parameter, not character number.
	    */
	    if (XftDrawStringUtf8 == xftdraw_string)
		loopitem <<= 1;
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "output entire mono string\n"));
	}
	/*
	** Non monospace font, but still we can improve the performance
	** by print it once under certain conditions
	*/
# ifdef MULTICHAR_SET
	else
	if (
	      NOTSET_OPTION(r, Opt2_xftSlowOutput)
	      && (XftDrawStringUtf8 == xftdraw_string)
	      && (
		  r->TermWin.xftmfont->max_advance_width ==
		  	(r->TermWin.fwidth << 1)
		 )
	   )
	{
	    /* print string once for multichar string */
	    loop = 1;
	    /*
	    ** If XftDrawStringUtf8 == xftdraw_string
	    **     string length == 2 * character number, but we need
	    ** to multiply loopitem by 2 because iconv need string
	    ** length as parameter, not character number.
	    */
	    loopitem = (len << 1);
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "output entire UTF-8 string\n"));
	}
	else
	if (
	      NOTSET_OPTION(r, Opt2_xftSlowOutput)
	      && (XftDrawString16 == xftdraw_string)
	      && (
		  r->TermWin.xftmfont->max_advance_width ==
		  	(r->TermWin.fwidth << 1)
		 )
	   )
	{
	    /* print string once for 16-bits string */
	    loop = 1;
	    loopitem = len;
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "output entire 16-bits string\n"));
	}
# endif	/* MULTICHAR_SET */
	else
	if (
	      r->TermWin.xftfnmono && (XftDrawString8 == xftdraw_string)
	      && (r->TermWin.xftfont->max_advance_width == r->TermWin.fwidth)
	   )
	{
	    /* print string once for 8-bits string */
	    loop = 1;
	    loopitem = len;
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "output entire 8-bits string\n"));
	}
	else
	{
	    /* print string one by one character */
	    loop = len;
	    loopitem = 1 + adjust;
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "output characters one by one\n"));
	}


	newstr = str;	/* string beginning in each iteration */
	for (i = 0; i < loop; i ++)
	{
# ifdef MULTICHAR_SET
#  ifdef HAVE_ICONV_H
	    if (XftDrawStringUtf8 == xftdraw_string)
	    {
		/* We should convert the string to UTF-8 */
		char*	buf = pbuf;		/* always init it */
		int	buflen = sizeof(pbuf)-1;/* always init it */
		int	newlen = loopitem;	/* always init it */
		char*	oldstr = newstr;
		iconv (r->TermWin.xfticonv, (char**)(&newstr),
		    (size_t*) &newlen, &buf, (size_t*) &buflen);
		*buf = (char) 0;    /* set end of string */
		pstr = pbuf;
		/*
		 * we should use the length of original string, not UTF-8 string
		 * here!!!
		 */
		plen = loopitem;
		/*
		 * reset newstr to old position, we will increase it later
		 */
		newstr = oldstr;
	    }
	    else
#  endif
# endif	/* MULTICHAR_SET */
	    {
		/* We do not need to convert the string to UTF-8 */
		pstr = newstr;
		plen = loopitem;
	    }

	    rxvt_draw_string_xft(r, PVTS( r, page)->vt, r->TermWin.gc,
		    refreshRegion, rend, NO_PFONT,
		    PVTS(r, page)->xftvt, &(r->xftColors[fore]),
		    x, y, pstr, plen, xftdraw_string);

	    x += Width2Pixel (loopitem);
	    newstr += loopitem;	/* next string to display */
	}
    }
    else
#endif	/* XFT_SUPPORT */
    {
	int	(*draw_string) ();
	switch (drawfunc)
	{
	    case    X11_DRAW_STRING_8:
		draw_string = XDrawString; break;
	    case    X11_DRAW_STRING_16:
		draw_string = XDrawString16; break;
	    case    X11_DRAW_IMAGE_STRING_8:
		draw_string = XDrawImageString; break;
	    case    X11_DRAW_IMAGE_STRING_16:
		draw_string = XDrawImageString16; break;

	    case    XFT_DRAW_STRING_8:	    /* fall back to X11 */
		draw_string = XDrawString; break;
	    case    XFT_DRAW_STRING_16:	    /* fall back to X11 */
	    case    XFT_DRAW_STRING_32:	    /* fall back to X11 */
	    case    XFT_DRAW_STRING_UTF8:   /* fall back to X11 */
		draw_string = XDrawString16; break;

	    case    XFT_DRAW_IMAGE_STRING_8:	/* fall back to X11 */
		draw_string = XDrawImageString; break;
	    case    XFT_DRAW_IMAGE_STRING_16:	/* fall back to X11 */
	    case    XFT_DRAW_IMAGE_STRING_32:	/* fall back to X11 */
	    case    XFT_DRAW_IMAGE_STRING_UTF8:	/* fall back to X11 */
		draw_string = XDrawImageString16; break;

	    default:
		draw_string = NULL; break;
	}

	/* We use TermWin.font->ascent here */
	y += r->TermWin.font->ascent;

	/* Now draw the string */
	if (draw_string)
	    rxvt_draw_string_x11 (r, PVTS(r, page)->vt, r->TermWin.gc,
		    refreshRegion, x, y, str, len, draw_string);