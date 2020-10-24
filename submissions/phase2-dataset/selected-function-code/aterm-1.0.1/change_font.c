change_font(int init, const char *fontname)
{
    const char     *const msg = "can't load font \"%s\"";
    XFontStruct    *xfont;
    static char    *newfont[NFONTS];

#ifndef NO_BOLDFONT
    static XFontStruct *boldFont = NULL;

#endif
    static int      fnum = FONT0_IDX;	/* logical font number */
    int             idx = 0;	/* index into rs_font[] */

#if (FONT0_IDX == 0)
# define IDX2FNUM(i)	(i)
# define FNUM2IDX(f)	(f)
#else
# define IDX2FNUM(i)	(i == 0 ? FONT0_IDX : (i <= FONT0_IDX ? (i-1) : i))
# define FNUM2IDX(f)	(f == FONT0_IDX ? 0 : (f < FONT0_IDX  ? (f+1) : f))
#endif
/*#define FNUM_RANGE(i)	(i <= 0 ? 0 : (i >= NFONTS ? (NFONTS-1) : i))*/
/* so to make it working on Sun Ultra  thanks to Hari Nair */
#define FNUM_RANGE(i)	(i < 0 ? (NFONTS-1) : (i >= NFONTS ? 0 : i))

    if (!init) {
	switch (fontname[0]) {
	case '\0':
	    fnum = FONT0_IDX;
	    fontname = NULL;
	    break;

	/* special (internal) prefix for font commands */
	case FONT_CMD:
	    idx = atoi(fontname + 1);
	    switch (fontname[1]) {
	    case '+':		/* corresponds to FONT_UP */
		fnum += (idx ? idx : 1);
		fnum = FNUM_RANGE(fnum);
		break;

	    case '-':		/* corresponds to FONT_DN */
		fnum += (idx ? idx : -1);
		fnum = FNUM_RANGE(fnum);
		break;

	    default:
		if (fontname[1] != '\0' && !isdigit(fontname[1]))
		    return;
		if (idx < 0 || idx >= (NFONTS))
		    return;
		fnum = IDX2FNUM(idx);
		break;
	    }
	    fontname = NULL;
	    break;

	default:
	    if (fontname != NULL) {
	    /* search for existing fontname */
		for (idx = 0; idx < NFONTS; idx++) {
		    if (!strcmp(rs_font[idx], fontname)) {
			fnum = IDX2FNUM(idx);
			fontname = NULL;
			break;
		    }
		}
	    } else
		return;
	    break;
	}
    /* re-position around the normal font */
	idx = FNUM2IDX(fnum);

	if (fontname != NULL) {
	    char           *name;

	    xfont = XLoadQueryFont(Xdisplay, fontname);
	    if (!xfont)
		return;

	    name = MALLOC(strlen(fontname + 1) * sizeof(char));

	    if (name == NULL) {
		XFreeFont(Xdisplay, xfont);
		return;
	    }
	    STRCPY(name, fontname);
	    if (newfont[idx] != NULL)
		FREE(newfont[idx]);
	    newfont[idx] = name;
	    rs_font[idx] = newfont[idx];
	}
    }
    if (TermWin.font)
	XFreeFont(Xdisplay, TermWin.font);

/* load font or substitute */
    xfont = XLoadQueryFont(Xdisplay, rs_font[idx]);
    if (!xfont) {
	print_error(msg, rs_font[idx]);
	rs_font[idx] = "fixed";
	xfont = XLoadQueryFont(Xdisplay, rs_font[idx]);
	if (!xfont) {
	    print_error(msg, rs_font[idx]);
	    goto Abort;
	}
    }
    TermWin.font = xfont;

#ifndef NO_BOLDFONT
/* fail silently */
    if (init && rs_boldFont != NULL)
	boldFont = XLoadQueryFont(Xdisplay, rs_boldFont);
#endif

#ifdef MULTICHAR_SET
    if (TermWin.mfont)
	XFreeFont(Xdisplay, TermWin.mfont);

/* load font or substitute */
    xfont = XLoadQueryFont(Xdisplay, rs_mfont[idx]);
    if (!xfont) {
	print_error(msg, rs_mfont[idx]);
	rs_mfont[idx] = "k14";
	xfont = XLoadQueryFont(Xdisplay, rs_mfont[idx]);
	if (!xfont) {
	    print_error(msg, rs_mfont[idx]);
	    goto Abort;
	}
    }
    TermWin.mfont = xfont;
#endif				/* MULTICHAR_SET */

/* alter existing GC */
    if (!init) {
	XSetFont(Xdisplay, TermWin.gc, TermWin.font->fid);
	menubar_expose();
    }
/* set the sizes */
    {
	int             fh, fw = 0;

	fw = TermWin.font->min_bounds.width; /* can be error !!!! */
	if( fw > 1000 ) fw = 0 ;  /* added by suggestion from <suchness>*/

#ifdef USE_LINESPACE
      fh = TermWin.font->ascent + TermWin.font->descent + TermWin.lineSpace;
#else
      fh = TermWin.font->ascent + TermWin.font->descent;
#endif

	if (TermWin.font->min_bounds.width == TermWin.font->max_bounds.width)
	    TermWin.fprop = 0;	/* Mono-spaced (fixed width) font */
	else
	    TermWin.fprop = 1;	/* Proportional font */

	if (TermWin.fprop == 1)	/* also, if == 0, per_char[] might be NULL */
	    fw = get_proportion_font_width( TermWin.font );
    /* not the first time thru and sizes haven't changed */
	if (fw == TermWin.fwidth && fh == TermWin.fheight)
	    return;		/* TODO: not return; check MULTI if needed */

	TermWin.fwidth = fw;
	TermWin.fheight = fh;
    }

/* check that size of boldFont is okay */
#ifndef NO_BOLDFONT
    TermWin.boldFont = NULL;
    if (boldFont != NULL) {
	int             fh, fw = 0;

	fw = boldFont->min_bounds.width; /* can be error !!!! */
	if( fw > 1000 ) fw = 0 ;

	fh = boldFont->ascent + boldFont->descent;
	if (TermWin.fprop == 0) {	/* bold font must also be monospaced */
	    if (fw != boldFont->max_bounds.width)
		fw = -1;
	} else
	    fw = get_proportion_font_width( boldFont );

	if (fw == TermWin.fwidth && fh == TermWin.fheight)
	    TermWin.boldFont = boldFont;
    }
#endif				/* NO_BOLDFONT */

/* TODO: check that size of Kanji font is okay */

    set_colorfgbg();

    TermWin.width = TermWin.ncol * TermWin.fwidth;
    TermWin.height = TermWin.nrow * TermWin.fheight;

    szHint.width_inc = TermWin.fwidth;
    szHint.height_inc = TermWin.fheight;

    szHint.min_width = szHint.base_width + szHint.width_inc;
    szHint.min_height = szHint.base_height + szHint.height_inc;

    szHint.width = szHint.base_width + TermWin.width;
    szHint.height = szHint.base_height + TermWin.height;
    { /* ONLYIF MENUBAR */
	szHint.height += (delay_menu_drawing ? menuBar_TotalHeight() : 0);
    }

    szHint.flags = PMinSize | PResizeInc | PBaseSize | PWinGravity;

    if (!init) {
	resize();
    }
    return;
  Abort:
    print_error("aborting");	/* fatal problem */
    exit(EXIT_FAILURE);
#undef IDX2FNUM
#undef FNUM2IDX
#undef FNUM_RANGE
}
