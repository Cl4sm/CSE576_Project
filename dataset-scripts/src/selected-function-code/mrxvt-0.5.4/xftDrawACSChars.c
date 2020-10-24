void
xftDrawACSChars(
	Display *dpy, Drawable d, GC gc,
	XftDraw *draw, const XftColor *color, XftFont *pub,
	int x, int y, const unsigned char *str, int len)
{

    /*
     * Line segments to draw line like chars.
     */
    static const short glyph_ht[] = {
	SEG(  0,	    0,		  0,	      5*MID_HIGH/6),	/* H */
	SEG(6*BOX_WIDE/10,  0,		6*BOX_WIDE/10,5*MID_HIGH/6),
	SEG(  0,	  5*MID_HIGH/12,6*BOX_WIDE/10,5*MID_HIGH/12),
	SEG(2*BOX_WIDE/10,  MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* T */
	SEG(6*BOX_WIDE/10,  MID_HIGH,	6*BOX_WIDE/10,	CHR_HIGH),
	-1
    }, glyph_ff[] = {
	SEG(  0,	    0,		6*BOX_WIDE/10,	0),		/* F */
	SEG(  0,	  5*MID_HIGH/12,6*CHR_WIDE/12,5*MID_HIGH/12),
	SEG(  0,	    0,		0*BOX_WIDE/3, 5*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* F */
	SEG(1*BOX_WIDE/3, 8*MID_HIGH/6,10*CHR_WIDE/12,8*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),
	-1
    }, glyph_lf[] = {
	SEG(  0,	    0,		  0,	      5*MID_HIGH/6),	/* L */
	SEG(  0,	  5*MID_HIGH/6,	6*BOX_WIDE/10,5*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* F */
	SEG(1*BOX_WIDE/3, 8*MID_HIGH/6,10*CHR_WIDE/12,8*MID_HIGH/6),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),
	-1
    }, glyph_nl[] = {
	SEG(  0,	  5*MID_HIGH/6,	  0,		0),		/* N */
	SEG(  0,	    0,		5*BOX_WIDE/6, 5*MID_HIGH/6),
	SEG(5*BOX_WIDE/6, 5*MID_HIGH/6, 5*BOX_WIDE/6,	0),
	SEG(1*BOX_WIDE/3,   MID_HIGH,	1*BOX_WIDE/3,	CHR_HIGH),	/* L */
	SEG(1*BOX_WIDE/3,   CHR_HIGH,	  CHR_WIDE,	CHR_HIGH),
	-1
    }, glyph_vt[] = {
	SEG(  0,	    0,		5*BOX_WIDE/12,5*MID_HIGH/6),	/* V */
	SEG(5*BOX_WIDE/12,5*MID_HIGH/6, 5*BOX_WIDE/6,	0),
	SEG(2*BOX_WIDE/10,  MID_HIGH,	  CHR_WIDE,	MID_HIGH),	/* T */
	SEG(6*BOX_WIDE/10,  MID_HIGH,	6*BOX_WIDE/10,	CHR_HIGH),
	-1
    }, lower_right_corner[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	0),
	-1
    }, upper_right_corner[] =
    {
	SEG(  0,	    MID_HIGH,	  MID_WIDE,	MID_HIGH),
	SEG( MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, upper_left_corner[] =
    {
	SEG(  MID_WIDE,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, lower_left_corner[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_WIDE,	  BOX_WIDE,	MID_HIGH),
	-1
    }, cross[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	-1
    }, left_tee[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	-1
    }, right_tee[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  0,		MID_HIGH),
	-1
    }, bottom_tee[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	MID_HIGH),
	-1
    }, top_tee[] =
    {
	SEG(  0,	    MID_HIGH,	  BOX_WIDE,	MID_HIGH),
	SEG(  MID_WIDE,	    MID_HIGH,	  MID_WIDE,	BOX_HIGH),
	-1
    }, vertical_line[] =
    {
	SEG(  MID_WIDE,	    0,		  MID_WIDE,	BOX_HIGH),
	-1
    };

    /*
     * Pointer to line-segment structure.
     */
    static const short *lines[] = {
	NULL,			/* 00 (unused) */
	NULL,			/* 01 diamond */
	NULL,			/* 02 box */
	glyph_ht,		/* 03 HT */
	glyph_ff,		/* 04 FF */
	NULL,			/* 05 CR (not drawn) */
	glyph_lf,		/* 06 LF */
	NULL,			/* 07 degrees (small circle) */
	NULL,			/* 08 plus or minus*/
	glyph_nl,		/* 09 */
	glyph_vt,		/* 0A */
	lower_right_corner,	/* 0B */
	upper_right_corner,	/* 0C */
	upper_left_corner,	/* 0D */
	lower_left_corner,	/* 0E */
	cross,			/* 0F */
	NULL,			/* 10 overline  */
	NULL,			/* 11 topline   */
	NULL,			/* 12 midline   */
	NULL,			/* 13 botline   */
	NULL,			/* 14 underline */
	left_tee,		/* 15 */
	right_tee,		/* 16 */
	bottom_tee,		/* 17 */
	top_tee,		/* 18 */
	vertical_line,		/* 19 */
	NULL,			/* 1A leq */
	NULL,			/* 1B geq */
	NULL,			/* 1C pi */
	NULL,			/* 1D neq */
	NULL,			/* 1E pound  */
	NULL,			/* 1F bullet */
    };

    /*
     * Character number in XftFont (if any).
     */
    static const FT_UInt xftCharNo[] = {
	0,	/* 00 (unused) */
	0,	/* 01 diamond */
	0,	/* 02 box */
	0,	/* 03 HT */
	0,	/* 04 FF */
	0x8b,	/* 05 CR (drawn as (c) ) */
	0,	/* 06 LF */
	0x83,	/* 07 degrees (small circle) */
	0x93,	/* 08 plus or minus*/
	0,	/* 09 */
	0,	/* 0A */
	0,	/* 0B */
	0,	/* 0C */
	0,	/* 0D */
	0,	/* 0E */
	0,	/* 0F */
	0,	/* 10 overline  */
	0,	/* 11 topline   */
	0,	/* 12 midline   */
	0,	/* 13 botline   */
	0,	/* 14 underline */
	0,	/* 15 */
	0,	/* 16 */
	0,	/* 17 */
	0,	/* 18 */
	0,	/* 19 */
	0x94,	/* 1A leq */
	0x95,	/* 1B geq */
	0x9b,	/* 1C pi */
	0x8f,	/* 1D neq */
	0x85,	/* 1E pound  */
	0x87	/* 1F bullet */
    };

    unsigned font_width	 = pub->max_advance_width;
    unsigned font_height = pub->ascent + pub->descent;
    const short *p;
    FT_UInt glyphs[MAX_GLYPHS];

    int ytop = y - pub->ascent; /* (x, ytop) is the top left corner */

    /*
     * Update fill styles in acsGc
     */
    XSetLineAttributes(dpy, acsGc, (font_height > 16) ? font_height / 16 : 1,
	LineSolid, CapProjecting, JoinMiter);
    XCopyGC( dpy, gc, GCForeground | GCBackground, acsGc);

    /*
     * Draw the characters. A few (ones with curves / shading) need to be
     * treated specially. The rest can be drawn with the segments in lines[n].
     */
    while( 1 )
    {
	/*
	 * If even one value in str[] is >= 32, this loop will never terminate.
	 * This is checked before calling, so no need to recheck here.
	 *
	 * We use such "contorted" code to optimize for speed. Since this is
	 * done several times while refreshing the screen, we don't want to slow
	 * things down.
	 */
	int n;

	/*
	 * Find max contiguous block of chars which are present in the Xft font,
	 * and use XftDrawGlyphs to draw them.
	 */
	for( n=0; n < len && n < MAX_GLYPHS && (glyphs[n] = xftCharNo[ *str ]);
		n++, str++);
	if( n )
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "(%d glyphs) ", n));

	    XftDrawGlyphs( draw, color, pub, x, y, glyphs, n);

	    x += n * font_width;
	    if( !(len -= n) ) break; /* !(len -= n) iff (len -= n) <= 0 */
	}

	/*
	 * Draw contiguous stippled box (0x02)
	 */
	for( n=0; n < len && *str == 2; n++, str++);
	if( n )
	{
	    XGCValues values;

	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "(%d boxes) ", n));

	    XGetGCValues( dpy, acsGc, GCForeground | GCBackground, &values);

	    XDrawPoint( dpy, sPmap, acsGc, 0, 0);
	    XDrawPoint( dpy, sPmap, acsGc, 1, 1);

	    XSetForeground( dpy, acsGc, values.background);
	    XDrawPoint( dpy, sPmap, acsGc, 0, 1);
	    XDrawPoint( dpy, sPmap, acsGc, 1, 0);

	    values.fill_style = FillTiled;
	    XChangeGC( dpy, acsGc, GCForeground | GCFillStyle, &values);

	    XFillRectangle( dpy, d, acsGc, x, ytop, n * font_width, font_height);

	    if( !(len -= n) ) break; /* !(len -= n) iff (len -= n) <= 0 */
	    x += n * font_width;
	}

	/*
	 * Draw contiguous horizontal lines.
	 */
	if( *str >= 0x10 && *str <= 0x14)
	{
	    unsigned char c = *str;
	    int xstart = x;
	    int ystart = ytop + ((c - 0x10) * (font_height-1)) / 4;

	    for( n=0; ++n < len && *(++str) == c; );

	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "(%d hln)", n));
	    x += n * font_width;

	    XSetFillStyle( dpy, acsGc, FillSolid);
	    XDrawLine( dpy, d, acsGc, xstart, ystart, x-1, ystart);

	    if( !(len -= n)) break;
	}

	/*
	 * Contiguous drawing not possible for these cases.
	 */
	if (*str == 1)			/* Filled diamond */
	{
	    XPoint points[4];
	    int npoints = 4, n;

	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "(1 dmd) "));

	    points[0].x = CHR_WIDE/2 + (BOX_WIDE - CHR_WIDE) / 2;
	    points[0].y = 0;

	    points[1].x = (BOX_WIDE - CHR_WIDE) / 2;
	    points[1].y = CHR_HIGH/2 + (BOX_HIGH - CHR_HIGH) / 2;

	    points[2].x = points[0].x;
	    points[2].y = CHR_HIGH + (BOX_HIGH - CHR_HIGH) / 2;

	    points[3].x = CHR_WIDE + (BOX_WIDE - CHR_WIDE) / 2;
	    points[3].y = points[1].y;

	    for (n = 0; n < npoints; n++)
	    {
		SCALE_X(points[n].x);
		SCALE_Y(points[n].y);

		points[n].x += x;
		points[n].y += ytop;
	    }

	    XSetFillStyle( dpy, acsGc, FillSolid);
	    XFillPolygon( dpy, d, acsGc, points, npoints, Convex, CoordModeOrigin);

	    if( ! (--len)) break;
	    str++;
	    x += font_width;
	}
	else if( NOT_NULL(p = lines[*str]))
	{
	    /*
	     * Draw character using segments in lines[*str]
	     */
	    int coord[4];
	    int n = 0;

	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_XFTACS, "(1 ldc) "));

	    XSetFillStyle( dpy, acsGc, FillSolid);
	    while (*p >= 0)
	    {
		coord[n++] = *p++;
		if (n == 4)
		{
		    SCALE_X(coord[0]);
		    SCALE_Y(coord[1]);
		    SCALE_X(coord[2]);
		    SCALE_Y(coord[3]);
		    XDrawLine( dpy, d, acsGc,
			x + coord[0], ytop + coord[1],
			x + coord[2], ytop + coord[3]);
		    n = 0;
		}
	    }

	    if( !(--len)) break;
	    str++;
	    x += font_width;
	}
    }
#ifdef DEBUG
    rxvt_dbgmsg ((DBG_DEBUG, DBG_XFTACS, "\n"));
#endif
}