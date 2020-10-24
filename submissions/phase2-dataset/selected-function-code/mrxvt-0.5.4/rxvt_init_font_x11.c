void
rxvt_init_font_x11 (rxvt_t *r)
{
    char*	    msg = "can't load font \"%s\"\n";
    XFontStruct*    xfont;
#ifndef NO_BOLDFONT
    XFontStruct*    bfont;
    int		    ckfont;
#endif
    int		    fh = 0, fw = 0;
    int		    idx = 0;		/* index into rs_font[] */


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, "rxvt_init_font_x11 \n"));


#ifdef XFT_SUPPORT
    /* Only load fixed font if we use freetype font */
    if (ISSET_OPTION(r, Opt_xft) && r->TermWin.xftfont)
    {
	rxvt_init_font_fixed (r);
	return;
    }
#endif


    r->h->fnum = FONT0_IDX;
    idx = FNUM2IDX(r->h->fnum);

    /* OK, now it's time to load the default font */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load font (%s)\n", r->h->rs[Rs_font+idx]));
    xfont = XLoadQueryFont (r->Xdisplay, r->h->rs[Rs_font+idx]);
    if (IS_NULL(xfont))
    {
	/* failed to load font */
	rxvt_msg( DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_font+idx] );

	/* try to load fixed font */
	r->h->rs[Rs_font+idx] = "fixed";
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load font (%s)\n", r->h->rs[Rs_font+idx]));
	xfont = XLoadQueryFont(r->Xdisplay, r->h->rs[Rs_font+idx]);
	if (IS_NULL(xfont))
	{
	    /* still failed to load font */
	    rxvt_msg( DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_font+idx] );

	    /* cannot load any font, fatal error, abort the program */
	    goto Abort;
	}
    }
    /* Font loading succeeded */
    if (NOT_NULL(xfont))
    {
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

    r->TermWin.fwidth  = fw;
    r->TermWin.fheight = fh;

#ifdef XFT_SUPPORT
    r->TermWin.pwidth	= fw;
    r->TermWin.pheight	= fh;
#endif

#ifndef NO_BOLDFONT
    SET_NULL(bfont);
    if (ckfont)
    {
	/* try to load boldFont, fail silently */
	if (NOT_NULL(r->h->rs[Rs_boldFont+idx]))
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load bfont (%s)\n", r->h->rs[Rs_boldFont+idx]));
	    bfont = XLoadQueryFont (r->Xdisplay, r->h->rs[Rs_boldFont+idx]);
	}

	if (NOT_NULL(bfont))
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
		    r->TermWin.propfont &= ~PROPFONT_BOLD;
		else
		    r->TermWin.propfont |= PROPFONT_BOLD;
	    }
	    else
	    {
		XFreeFont (r->Xdisplay, bfont);
	    }
	}
    }
#endif /* NO_BOLDFONT */

#ifdef MULTICHAR_SET
    /* load font or substitute */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load mfont (%s)\n", r->h->rs[Rs_mfont+idx]));
    xfont = XLoadQueryFont(r->Xdisplay, r->h->rs[Rs_mfont+idx]);
    if (IS_NULL(xfont))
    {
	char*	ptr;

	/* failed to load font */
	rxvt_msg (DBG_ERROR, DBG_MAIN, msg, r->h->rs[Rs_mfont+idx]);

	ptr = rxvt_fallback_mfont_x11 (r);
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_MAIN, " load mfont (%s)\n", ptr));
	xfont = XLoadQueryFont(r->Xdisplay, ptr);
	if (NOT_NULL(xfont))
	    r->h->rs[Rs_mfont+idx] = ptr;
	else
	{
	    /* still failed to load font */
	    rxvt_msg (DBG_ERROR, DBG_MAIN, msg, ptr);
	    /* cannot load any mfont, fatal error, abort the program */
	    goto Abort;
	}
    }
    if (NOT_NULL(xfont))
    {
	r->TermWin.mfont = xfont;
    }
#endif	/* MULTICHAR_SET */

    /* Succeeded to load font, return now */
    return ;

Abort:
    rxvt_msg (DBG_ERROR, DBG_MAIN, "fatal error, aborting...");
    exit(EXIT_FAILURE);
}