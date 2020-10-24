void
scr_rendition(int set, int style)
{
    unsigned int    color;
    rend_t          font_attr;

    if (set) {
/* A: Set style */
	rstyle |= style;
	switch (style) {
	case RS_RVid:
	    if (rvideo)
		rstyle &= ~RS_RVid;
	    break;
#ifndef NO_BRIGHTCOLOR
	case RS_Bold:
	    color = GET_FGCOLOR(rstyle);
	    scr_color((color == Color_fg ? GET_FGCOLOR(colorfgbg) : color),
		      RS_Bold);
	    break;
	case RS_Blink:
	    color = GET_BGCOLOR(rstyle);
	    scr_color((color == Color_bg ? GET_BGCOLOR(colorfgbg) : color),
		      RS_Blink);
	    break;
#endif
	}
    } else {
/* B: Unset style */
	font_attr = rstyle & RS_fontMask;
	rstyle &= ~style;

	switch (style) {
	case ~RS_None:		/* default fg/bg colours */
	    rstyle = DEFAULT_RSTYLE | font_attr;
	/* FALLTHROUGH */
	case RS_RVid:
	    if (rvideo)
		rstyle |= RS_RVid;
	    break;
#ifndef NO_BRIGHTCOLOR
	case RS_Bold:
	    color = GET_FGCOLOR(rstyle);
	    if (color >= minBrightCOLOR && color <= maxBrightCOLOR) {
		scr_color(color, RS_Bold);
		if ((rstyle & RS_fgMask) == (colorfgbg & RS_fgMask))
		    scr_color(restoreFG, RS_Bold);
	    }
	    break;
	case RS_Blink:
	    color = GET_BGCOLOR(rstyle);
	    if (color >= minBrightCOLOR && color <= maxBrightCOLOR) {
		scr_color(color, RS_Blink);
		if ((rstyle & RS_bgMask) == (colorfgbg & RS_bgMask))
		    scr_color(restoreBG, RS_Blink);
	    }
	    break;
#endif
	}
    }
}
