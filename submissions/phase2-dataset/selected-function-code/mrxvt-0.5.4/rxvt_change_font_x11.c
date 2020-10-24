int
rxvt_change_font_x11 (rxvt_t* r, const char *fontname)
{
    char*	    msg = "can't load font \"%s\"";
    XFontStruct*    xfont;
#ifndef NO_BOLDFONT
    XFontStruct*    bfont;
    int		    ckfont;
#endif
    int		    fh = 0, fw = 0;
    int		    idx = 0;		/* index into rs_font[] */


    assert (fontname);
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, "rxvt_change_font_x11 (%s)\n", fontname));


    switch (fontname[0])
    {
	/* special (internal) prefix for font commands */
	case FONT_CMD: /* FONT_CMD =='#' */
	    idx = atoi (fontname + 1);

	    switch (fontname[1])
	    {
		case '+':   /* corresponds to FONT_UP */
		    r->h->fnum += (idx ? idx : 1);  /* "#+" or "#+3"? */
		    r->h->fnum %= MAX_NFONTS;
		    break;

		case '-':   /* corresponds to FONT_DN */
		    r->h->fnum += (idx ? idx : -1); /* "#-" or "#-3"? */
		    r->h->fnum %= MAX_NFONTS;
		    break;

		default:
		    /* input is not a logical font number */
		    if (fontname[1] != '\0' &&
			!isdigit((int) fontname[1]))
			return 0;
		    /*
		     * input logical font number too big, but don't worry, we
		     * will handle it gracefully ;-)
		     */
		    r->h->fnum = IDX2FNUM(idx);
		    break;
	    }
	    SET_NULL(fontname);
	    break;

	default:
	    /* search for existing fontname */
	    for (idx = 0; idx < MAX_NFONTS; idx++)
	    {
		if (!STRCMP (r->h->rs[Rs_font+idx], fontname))
		{
		    r->h->fnum = IDX2FNUM(idx);
		    SET_NULL(fontname);
		    break;
		}
	    }
	    break;
    }
    /* re-position around the normal font */
    if (r->h->fnum < 0)
	r->h->fnum += (-(r->h->fnum / MAX_NFONTS - 1) * MAX_NFONTS);
    idx = FNUM2IDX(r->h->fnum);


    /*
    ** If fontname != NULL, it's some new font not in the rs_font.
    ** We try to load it and replace font in rs_font if succeed.
    */
    if (NOT_NULL(fontname))
    {
	xfont = XLoadQueryFont(r->Xdisplay, fontname);
	if (xfont)
	{
	    /* load new font succeessfully */
	    char* ptr = STRDUP (fontname);
	    if (ptr)
	    {
		if (NOT_NULL(r->h->newfont[idx]))   
		    rxvt_free (r->h->newfont[idx]);
		r->h->newfont[idx] = ptr;
		r->h->rs[Rs_font+idx] = r->h->newfont[idx];
	    }
	    else
	    {
		assert (0); /* shouldn't happen */
	    }
	    /* Free it by now */
	    XFreeFont (r->Xdisplay, xfont);
	}
    }


    /*
    ** OK, now it's time to load font or substitute
    */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load font (%s)\n", r->h->rs[Rs_font+idx]));
    xfont = XLoadQueryFont (r->Xdisplay, r->h->rs[Rs_font+idx]);
    if (!xfont)
    {
	/* failed to load font */
	rxvt_msg (DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_font+idx]);

	/* try to load fixed font */
	r->h->rs[Rs_font+idx] = "fixed";
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load font (%s)\n", r->h->rs[Rs_font+idx]));
	xfont = XLoadQueryFont(r->Xdisplay, r->h->rs[Rs_font+idx]);
	if (!xfont)
	{
	    /* still failed to load font */
	    rxvt_msg (DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_font+idx]);
	    return 0;
	}
    }
    /* Font loading succeeded */
    if (xfont)
    {
	/*
	** if the previous font exists, replace it with new font.
	** otherwise, keep the old font, and do nothing
	*/
	if (r->TermWin.font)
	    XFreeFont (r->Xdisplay, r->TermWin.font);
	r->TermWin.font = xfont;
    }


    /* set the font sizes */
    fw = rxvt_get_font_widest (r->TermWin.font);
    fh = r->TermWin.font->ascent + r->TermWin.font->descent;
#ifndef NO_LINESPACE
    fh += r->TermWin.lineSpace;
#endif

    if (fw == r->TermWin.font->min_bounds.width)
	/* Mono-spaced (fixed width) font */
	r->TermWin.propfont &= ~PROPFONT_NORMAL;
    else
	/* Proportional font */
	r->TermWin.propfont |= PROPFONT_NORMAL;

#ifndef NO_BOLDFONT
    ckfont = !(fw == r->TermWin.fwidth && fh == r->TermWin.fheight);
#endif

#ifdef XFT_SUPPORT
    /* Set font size when XFT is not enabled */
    if (!(ISSET_OPTION(r, Opt_xft) && r->TermWin.xftfont))
#endif
    {
#ifdef XFT_SUPPORT
	r->TermWin.pwidth  = fw;
	r->TermWin.pheight = fh;
#endif
	r->TermWin.fwidth  = fw;
	r->TermWin.fheight = fh;
    }


#ifndef NO_BOLDFONT
    SET_NULL(bfont);
    if (ckfont)
    {
	if (r->TermWin.bfont)	{
	    XFreeFont (r->Xdisplay, r->TermWin.bfont);
	    SET_NULL(r->TermWin.bfont);
	}

	/* try to load boldFont, fail silently */
	if (IS_NULL(r->TermWin.bfont) &&
	    NOT_NULL(r->h->rs[Rs_boldFont+idx]))
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load bfont (%s)\n", r->h->rs[Rs_boldFont+idx]));
	    bfont = XLoadQueryFont (r->Xdisplay, r->h->rs[Rs_boldFont+idx]);
	}

	if (bfont)
	{
	    /* Loading bold font succeeded */
	    fw = rxvt_get_font_widest (bfont);
	    fh = bfont->ascent + bfont->descent;
#ifndef NO_LINESPACE
	    fh += r->TermWin.lineSpace;
#endif
	    if (fw <= r->TermWin.fwidth && fh <= r->TermWin.fheight)
	    {
		r->TermWin.bfont = bfont;
		if (fw == r->TermWin.fwidth)
		    r->TermWin.propfont &= ~PROPFONT_NORMAL;
		else
		    r->TermWin.propfont |= PROPFONT_NORMAL;
	    }
	    else
	    {
		XFreeFont (r->Xdisplay, bfont);
	    }
	}
    }
#endif /* NO_BOLDFONT */

#ifdef HAVE_MENUBAR
    /*
     * 2006-01-29 gi1242: We should do something to resize the menubar font.
     * However just changing it in the appropriate GC from here doesn't seem to
     * work at all.
     */
    /* Resize the menubar font too. Only needed for X11 font. */
    if(
#ifdef XFT_SUPPORT
	NOTSET_OPTION(r, Opt_xft) &&
#endif
	IS_WIN(r->menuBar.win))
    {
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, "Resized menubar font\n"));
	XSetFont( r->Xdisplay, r->menuBar.gc, r->TermWin.font->fid);
    }
#endif

#ifdef MULTICHAR_SET
    /* load font or substitute */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load mfont (%s)\n", r->h->rs[Rs_mfont+idx]));
    xfont = XLoadQueryFont(r->Xdisplay, r->h->rs[Rs_mfont+idx]);
    if (!xfont)
    {
	char*	ptr;

	/* failed to load font */
	rxvt_msg (DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_mfont+idx]);

	/* try to load default font */
	ptr = rxvt_fallback_mfont_x11 (r);
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load mfont (%s)\n", ptr));
	xfont = XLoadQueryFont(r->Xdisplay, ptr);
	if (xfont)
	    r->h->rs[Rs_mfont+idx] = ptr;
	else
	{
	    /* still failed to load font */
	    rxvt_msg (DBG_ERROR, DBG_MAIN, msg, ptr);
	    return 0;
	}
    }
    if (xfont)
    {
	/*
	** if the previous font exists, replace it with new font.
	** otherwise, keep the old font, and do nothing
	*/
	if (r->TermWin.mfont)
	    XFreeFont (r->Xdisplay, r->TermWin.mfont);
	r->TermWin.mfont = xfont;
    }
#endif	/* MULTICHAR_SET */

#ifdef USE_XIM
    rxvt_IM_change_fontset(r, idx);
#endif
    return 1;
}