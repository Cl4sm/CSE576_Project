scr_refresh(int type)
{
    int             i, j,	/* tmp                                       */
                    col, row,	/* column/row we're processing               */
                    scrrow,	/* screen row offset                         */
                    row_offset,	/* basic offset in screen structure          */
		    currow,	/* cursor row at appropriate offset          */
                    boldlast,	/* last character in some row was bold       */
                    len, wlen,	/* text length screen/buffer                 */
                    fprop,	/* proportional font used                    */
                    is_cursor,	/* cursor this position                      */
                    rvid,	/* reverse video this position               */
                    rend,	/* rendition                                 */
                    fore, back,	/* desired foreground/background             */
                    wbyte,	/* we're in multibyte                        */
		    morecur = 0,
                    xpixel,	/* x offset for start of drawing (font)      */
                    ypixel = 0;	/* y offset for start of drawing (font)      */

    static int      focus = -1;	/* screen in focus?                          */
    long            gcmask;	/* Graphics Context mask                     */
    unsigned long   ltmp;
    rend_t          rt1, rt2;	/* tmp rend values                           */
#ifndef NO_CURSORCOLOR
    rend_t	    ccol1,	/* Cursor colour                             */
		    ccol2,	/* Cursor colour2                            */
		    cc1 = 0;	/* store colours at cursor position(s)       */
# ifdef MULTICHAR_SET
    rend_t	    cc2 = 0;	/* store colours at cursor position(s)       */
# endif
#endif
    rend_t         *drp, *srp;	/* drawn-rend-pointer, screen-rend-pointer   */
    text_t         *dtp, *stp;	/* drawn-text-pointer, screen-text-pointer   */
    XGCValues       gcvalue;	/* Graphics Context values                   */
    XFontStruct    *wf;		/* which font are we in                      */
    static char    *buffer = NULL;
    static int      currmaxcol = 0;
#ifdef MULTICHAR_SET
    static int	    oldcursormulti = 0;
#endif
    static row_col_t oldcursor = { -1, -1};
    				/* is there an old outline cursor on screen? */

#ifdef THAI  /* variables to clean un-touched space at the end of screen */
    int		    thaicol_stp_offset = 0;
    int		    thaicol_dtp_offset = 0;
#endif

#ifndef NO_BOLDFONT
    int             bfont = 0;	/* we've changed font to bold font           */
#endif
    int             (*draw_string) (),
		    (*draw_image_string) ();

    if (type == NO_REFRESH)
	return;

/*
 * A: set up vars
 */
    if (currmaxcol < TermWin.ncol) {
	currmaxcol = TermWin.ncol;
	if (buffer)
	    buffer = REALLOC(buffer, (sizeof(char) * (currmaxcol + 1)));
	else
	    buffer = MALLOC((sizeof(char) * (currmaxcol + 1)));
    }
    row_offset = TermWin.saveLines - TermWin.view_start;
    fprop = TermWin.fprop;
    is_cursor = 0;
    gcvalue.foreground = PixColors[Color_fg];
    gcvalue.background = PixColors[Color_bg];
    dtp = stp = NULL;

/*
 * always go back to the base font - it's much safer
 */
    wbyte = 0;
    XSetFont(Xdisplay, TermWin.gc, TermWin.font->fid);
    draw_string = XDrawString;
    draw_image_string = XDrawImageString;
    boldlast = 0;
    wf = TermWin.font;

#ifndef NO_BOLDOVERSTRIKE
/*
 * B: Bold Overstrike pixel dropping avoidance.  Do this FIRST.
 *    Do a pass across each line at the start, require a refresh of anything
 *    that will need to be refreshed, due to pixels being dropped into our
 *    area by a previous character which has now been changed.
 */
    for (row = 0; row < TermWin.nrow; row++) {
	scrrow = row + row_offset;
	stp = screen.text[scrrow];
	srp = screen.rend[scrrow];
	dtp = drawn_text[row];
	drp = drawn_rend[row];
# ifndef NO_BOLDFONT
	if (TermWin.boldFont == NULL) {
# endif
	    j = wbyte;
	    for (col = TermWin.ncol - 2; col >= 0; col--) {
	      int do_stuff ;
# if ! defined (NO_BRIGHTCOLOR) && ! defined (VERYBOLD)
		fore = GET_FGCOLOR(drp[col]);
# endif
		if (!MONO_BOLD(drp[col]))
		    continue;
		if (dtp[col] == stp[col]
		    && drp[col] == srp[col])
		    continue;
		if (wbyte) {
		    ;		/* TODO: handle multibyte */
		    continue;	/* don't go past here */
		}
		if (dtp[col] == ' ') {	/* TODO: check character set? */
		    continue;
		}

		if( wf->per_char == NULL )
		    do_stuff = 1 ;
		else if( dtp[col] >= wf->min_char_or_byte2 &&
		    dtp[col] <  wf->max_char_or_byte2)
		{
		  int char_num = dtp[col]-wf->min_char_or_byte2 ;
		    do_stuff = (wf->per_char[char_num].width == wf->per_char[char_num].rbearing);
		}
		else
		    do_stuff = 0 ;

		if (do_stuff)
		{
#ifndef THAI    /* this mess up Thai chars, we have to implement ourselves */
		    dtp[col + 1] = 0;
#endif
# if defined(MULTICHAR_SET) && ! defined(NO_BOLDOVERSTRIKE_MULTI)
		    if ((srp[col] & RS_multiMask) == RS_multi2) {
		        col--;
		        wbyte = 1;
		        continue;
		    }
# endif
		}
	    }
# if ! defined (NO_BRIGHTCOLOR) && ! defined (VERYBOLD)
	    fore = GET_FGCOLOR(srp[TermWin.ncol - 1]);
# endif
	    if (MONO_BOLD(srp[TermWin.ncol - 1]))
		boldlast = 1;
	    wbyte = j;
# ifndef NO_BOLDFONT
	}
# endif
    }
#endif				/* ! NO_BOLDOVERSTRIKE */

/*
 * C: reverse any characters which are selected
 */
    scr_reverse_selection();

/*
 * D: set the cursor character(s)
 */
    currow = screen.cur.row + TermWin.saveLines;
    if (focus != TermWin.focus)
	focus = TermWin.focus;
    if (screen.flags & Screen_VisibleCursor && focus)
	{
		srp = &(screen.rend[currow][screen.cur.col]);
#ifdef THAI
		if(CharWidth(wf,screen.text[currow][screen.cur.col])<=0)
			*srp ^= RS_Bold;
#endif
#ifndef NO_CURSORCOLOR
		cc1 = *srp & (RS_fgMask | RS_bgMask);
		if (Xdepth <= 2 || !rs_color[Color_cursor])
	    	ccol1 = Color_fg;
		else
	    	ccol1 = Color_cursor;
		if (Xdepth <= 2 || !rs_color[Color_cursor2])
	    	ccol2 = Color_bg;
		else
	    	ccol2 = Color_cursor2;
	/*  changed by rev vide cursor patch */
		*srp = SET_BGCOLOR(*srp, ccol1);
		*srp = SET_FGCOLOR(*srp, ccol2);
#endif
#ifdef MULTICHAR_SET
		rt1 = *srp & RS_multiMask;
		if (rt1 == RS_multi1) {
	    	if (screen.cur.col < TermWin.bcol - 2
			&& ((srp[1] & RS_multiMask) == RS_multi2))
			morecur = 1;
		} else if (rt1 == RS_multi2) {
	    	if (screen.cur.col > 0
			&& ((srp[-1] & RS_multiMask) == RS_multi1))
			morecur = -1;
		}
		if (morecur) {
	    	srp += morecur;
	    	*srp ^= RS_RVid;
		}
# ifndef NO_CURSORCOLOR
		if (morecur) {
	    	cc2 = *srp & (RS_fgMask | RS_bgMask);
	    	*srp = SET_BGCOLOR(*srp, ccol1);
	    	*srp = SET_FGCOLOR(*srp, ccol2);
		}
# endif
#endif
    }
    i = 0;
    if (oldcursor.row != -1) {
    /* make sure no outline cursor is left around */
	if (screen.cur.row != oldcursor.row
	    || screen.cur.col != oldcursor.col) {
	    if (oldcursor.row < TermWin.nrow && oldcursor.col < TermWin.bcol) {
		drawn_text[oldcursor.row][oldcursor.col] = 0;
#ifdef MULTICHAR_SET
		if (oldcursormulti) {
		    col = oldcursor.col + oldcursormulti;
		    if (col < TermWin.ncol)
			drawn_text[oldcursor.row][col] = 0;
		}
#endif
	    }
	    if (focus || !(screen.flags & Screen_VisibleCursor))
		oldcursor.row = -1;
	    else
		i = 1;
	}
    } else if (!focus)
	i = 1;
    if (i) {
	oldcursor.row = screen.cur.row;
	oldcursor.col = screen.cur.col;
#ifdef MULTICHAR_SET
	oldcursormulti = morecur;
#endif
    }

/*
 * E: OK, now the real pass
 */
    for (row = 0; row < TermWin.nrow; row++) {
#ifdef THAI
        char thai_update[TermWin.ncol];    /* update map */
	/* compare drawn_text and screen.text and check which to update */
	ThaiUpdateMap(screen.text[row+row_offset],drawn_text[row],
		      screen.rend[row+row_offset],drawn_rend[row],
 		      thai_update,TermWin.ncol);

/*
 * TODO: Change this algo to scalefont compatible
 */
        thaicol_stp_offset = 0;  /* records how many column deficit */
        thaicol_dtp_offset = TermWin.ncol - Thai_ColMaxPaint(dtp,TermWin.ncol);
        /* clean strings before redraw */
        for(col = 0; col < TermWin.ncol; col++){
            if(!ThaiIsMiddleLineCh(stp[col])) thaicol_stp_offset++;
        }

        if(thaicol_stp_offset > thaicol_dtp_offset){
          CLEAR_CHARS(Col2Pixel( TermWin.ncol - thaicol_stp_offset ),
                      ypixel - TermWin.font->ascent, thaicol_stp_offset);

        }
#endif
	scrrow = row + row_offset;
	stp = screen.text[scrrow];
	srp = screen.rend[scrrow];
	dtp = drawn_text[row];
	drp = drawn_rend[row];
	for (col = 0; col < TermWin.ncol; col++) {
	/* compare new text with old - if exactly the same then continue */
	    rt1 = srp[col];	/* screen rendition */
	    rt2 = drp[col];	/* drawn rendition  */
#ifdef THAI  /* updating conditional */
	    if (!thai_update[col]) {
#else
	    if ((stp[col] == dtp[col])	/* must match characters to skip */
		&& ((rt1 == rt2)	/* either rendition the same or  */
		    || ((stp[col] == ' ')	/* space w/ no bg change */
			&& (GET_BGATTR(rt1) == GET_BGATTR(rt2))))) {
#endif
#ifdef MULTICHAR_SET
	    /* if first byte is Kanji then compare second bytes */
		if ((rt1 & RS_multiMask) != RS_multi1)
		    continue;
		else if (stp[col + 1] == dtp[col + 1]) {
		/* assume no corrupt characters on the screen */
		    col++;
		    continue;
		}
#else
		continue;
#endif
	    }
	/* redraw one or more characters */
#ifndef THAI  /* can not set dtp as we go because it causes
               * unpredictable results (bug)
               */
	    dtp[col] = stp[col];
#endif
	    rend = drp[col] = srp[col];

	    len = 0;
	    buffer[len++] = stp[col];
	    ypixel = TermWin.font->ascent + Row2Pixel(row);
#ifdef THAI   /* compute x coordinate */
	    xpixel = ThaiCol2Pixel( col, screen.text[scrrow]);
#else
	    xpixel = Col2Pixel(col);
#endif /* THAI */
	    wlen = 1;

/*
 * Find out the longest string we can write out at once
 */
	    if (fprop == 0) {	/* Fixed width font */
#ifdef MULTICHAR_SET
		if (((rend & RS_multiMask) == RS_multi1)
		    && col < TermWin.ncol - 1
		    && ((srp[col + 1]) & RS_multiMask) == RS_multi2) {
		    if (!wbyte) {
			wbyte = 1;
			XSetFont(Xdisplay, TermWin.gc, TermWin.mfont->fid);
			draw_string = XDrawString16;
			draw_image_string = XDrawImageString16;
		    }
		/* double stepping - we're in Kanji mode */
		    for (; ++col < TermWin.bcol;) {
		    /* XXX: could check sanity on 2nd byte */
			dtp[col] = stp[col];
			drp[col] = srp[col];
			buffer[len++] = stp[col];
			col++;
			if ((col == TermWin.bcol) || (srp[col] != rend))
			    break;
			if ((stp[col] == dtp[col])
			    && (srp[col] == drp[col])
			    && (stp[col + 1] == dtp[col + 1]))
			    break;
			if (len == currmaxcol)
			    break;
			dtp[col] = stp[col];
			drp[col] = srp[col];
			buffer[len++] = stp[col];
		    }
		    col--;
		    if (buffer[0] & 0x80)
			multichar_decode(buffer, len);
		    wlen = len / 2;
		} else {
		    if ((rend & RS_multiMask) == RS_multi1) {
		    /* XXX : maybe do the same thing for RS_multi2 */
		    /* corrupt character - you're outta there */
			rend &= ~RS_multiMask;
			drp[col] = rend;	/* TODO check: may also want */
			dtp[col] = ' ';		/* to poke into stp/srp      */
			buffer[0] = ' ';
		    }
		    if (wbyte) {
			wbyte = 0;
			XSetFont(Xdisplay, TermWin.gc, TermWin.font->fid);
			draw_string = XDrawString;
			draw_image_string = XDrawImageString;
		    }
#endif
		/* single stepping - `normal' mode */
		    for (; ++col < TermWin.ncol - 1;) {
			if (rend != srp[col])
			    break;
			if ((stp[col] == dtp[col]) && (srp[col] == drp[col]))
			    break;
			if (len == currmaxcol)
			    break;
			dtp[col] = stp[col];
			drp[col] = srp[col];
			buffer[len++] = stp[col];
		    }
		    col--;
		    wlen = len;
#ifdef MULTICHAR_SET
		}
#endif
	    }
	    buffer[len] = '\0';

/*
 * Determine the attributes for the string
 */
	    fore = GET_FGCOLOR(rend);
	    back = GET_BGCOLOR(rend);
	    rend = GET_ATTR(rend);
	    gcmask = 0;
	    rvid = (rend & RS_RVid) ? 1 : 0;

	    switch (rend & RS_fontMask) {
	    case RS_acsFont:
		for (i = 0; i < len; i++)
		    if (buffer[i] == 0x5f)
			buffer[i] = 0x7f;
		    else if (buffer[i] > 0x5f && buffer[i] < 0x7f)
			buffer[i] -= 0x5f;
		break;
	    case RS_ukFont:
		for (i = 0; i < len; i++)
		    if (buffer[i] == '#')
			buffer[i] = 0x1e;
		break;
	    }
	    if (rvid)
		SWAP_IT(fore, back, i);

	    if( colors_changed)
	    {
		gcvalue.background = PixColors[back];
		gcmask |= GCBackground;
		gcvalue.foreground = PixColors[fore];
		gcmask |= GCForeground;
		colors_changed = 0 ;
	    }else if (back != Color_bg) {
		    gcvalue.background = PixColors[back];
		    gcmask |= GCBackground;
		}
	    if (fore != Color_fg) {
	    	    gcvalue.foreground = PixColors[fore];
	    	    gcmask |= GCForeground;
	    }
#ifndef NO_BOLDUNDERLINE
	    else if (rend & RS_Bold) {
		if (Xdepth > 2 && rs_color[Color_BD]
		    && PixColors[fore] != PixColors[Color_BD]
		    && PixColors[back] != PixColors[Color_BD]) {
		    gcvalue.foreground = PixColors[Color_BD];
		    gcmask |= GCForeground;
# ifndef VERYBOLD
		    rend &= ~RS_Bold;	/* we've taken care of it */
# endif
		}
	    } else if (rend & RS_Uline) {
		if (Xdepth > 2 && rs_color[Color_UL]
		    && PixColors[fore] != PixColors[Color_UL]
		    && PixColors[back] != PixColors[Color_UL]) {
		    gcvalue.foreground = PixColors[Color_UL];
		    gcmask |= GCForeground;
		    rend &= ~RS_Uline;	/* we've taken care of it */
		}
	    }
#endif

	    if (gcmask)
		XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);

#ifndef NO_BOLDFONT
	    if (!wbyte && MONO_BOLD(rend) && TermWin.boldFont != NULL) {
		bfont = 1;
		XSetFont(Xdisplay, TermWin.gc, TermWin.boldFont->fid);
		rend &= ~RS_Bold;	/* we've taken care of it */
	    } else if (bfont) {
		bfont = 0;
		XSetFont(Xdisplay, TermWin.gc, TermWin.font->fid);
	    }
#endif
/*
 * Actually do the drawing of the string here
 */
	    if (fprop) {
#ifdef THAI
		if (rvid && CharWidth(wf,stp[col])>0) {
#else
		if (rvid) {
#endif
		    SWAP_IT(gcvalue.foreground, gcvalue.background, ltmp);
		    gcmask |= (GCForeground | GCBackground);
		    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
		    XFillRectangle(Xdisplay, drawBuffer, TermWin.gc,
				   xpixel, ypixel - TermWin.font->ascent,
				   Width2Pixel(1), Height2Pixel(1));
		    SWAP_IT(gcvalue.foreground, gcvalue.background, ltmp);
		    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
		} else
#ifdef THAI
 		   if(CharWidth(wf,stp[col])>0)
#endif
		    CLEAR_CHARS(xpixel, ypixel - TermWin.font->ascent, 1);
		DRAW_STRING(draw_string, xpixel, ypixel, buffer, 1);
#ifdef THAI
/*
 * Trap error
 */
 		      if(wlen > 1) fprintf(stderr, "wlen > 1: %d\n", wlen);
#endif
#ifndef NO_BOLDOVERSTRIKE
		if (MONO_BOLD(rend))
		    DRAW_STRING(draw_string, xpixel + 1, ypixel, buffer, 1);
#endif
	    } else
#ifdef TRANSPARENT
	    if ((Options & Opt_transparent) && back == Color_bg) {
#ifdef THAI
	        if(CharWidth(wf,stp[col])>0)
#endif
		CLEAR_CHARS(xpixel, ypixel - TermWin.font->ascent, len);
		DRAW_STRING(draw_string, xpixel, ypixel, buffer, wlen);
	    } else
#endif
#if defined(BACKGROUND_IMAGE) || defined(_MYSTYLE_)
	    if (TermWin.background.trgPixmapSet && back == Color_bg) {
		CLEAR_CHARS(xpixel, ypixel - TermWin.font->ascent, len);
		DRAW_STRING(draw_string, xpixel, ypixel, buffer, wlen);
	    } else
#endif
		DRAW_STRING(draw_image_string, xpixel, ypixel, buffer, wlen);

#ifndef NO_BOLDOVERSTRIKE
# ifdef NO_BOLDOVERSTRIKE_MULTI
	    if (!wbyte)
# endif
		if (MONO_BOLD(rend))
		    DRAW_STRING(draw_string, xpixel + 1, ypixel, buffer, wlen);
#endif
	    if ((rend & RS_Uline) && (TermWin.font->descent > 1))
		XDrawLine(Xdisplay, drawBuffer, TermWin.gc,
			  xpixel, ypixel + 1,
			  xpixel + Width2Pixel(len) - 1, ypixel + 1);
	    if (gcmask) {	/* restore normal colours */
		gcvalue.foreground = PixColors[Color_fg];
		gcvalue.background = PixColors[Color_bg];
		XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
	    }
	}
    }
#ifdef THAI
/*
 * Hack: update drawn_text
 */
	for (row = 0; row < TermWin.nrow; row++)
	{
		scrrow = row + row_offset;
		stp = screen.text[scrrow];
		srp = screen.rend[scrrow];
		dtp = drawn_text[row];
		drp = drawn_rend[row];

		for(col=0 ; col<TermWin.ncol ; col++)
		{
			dtp[col] = stp[col];
			drp[col] = srp[col];
		}
	}
#endif

/*
 * F: cleanup cursor and display outline cursor in necessary
 */
    if (screen.flags & Screen_VisibleCursor)
	{
		if (focus)
		{
	    	srp = &(screen.rend[currow][screen.cur.col]);
#ifdef THAI
	    	if (CharWidth(wf,screen.text[currow][screen.cur.col])<=0)
	        	*srp ^= RS_Bold;
#endif
#ifndef NO_CURSORCOLOR
	    	*srp = (*srp & ~(RS_fgMask | RS_bgMask)) | cc1;
#endif
	    	if (morecur) {
				srp += morecur;
#if defined(MULTICHAR_SET) && ! defined(NO_CURSORCOLOR)
			*srp = (*srp & ~(RS_fgMask | RS_bgMask)) | cc2;
#endif
	    }
	} else {
	    currow = screen.cur.row - TermWin.view_start;
	    col = screen.cur.col + morecur;
	    wbyte = morecur ? 1 : 0;
	    if (currow >= 0 && currow < TermWin.nrow) {
#ifndef NO_CURSORCOLOR
		gcmask = 0;
		if (Xdepth > 2 && rs_color[Color_cursor]) {
		    gcvalue.foreground = PixColors[Color_cursor];
		    gcmask = GCForeground;
		    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
		    gcvalue.foreground = PixColors[Color_fg];
		}
#endif
#ifdef THAI
		XDrawRectangle(Xdisplay, drawBuffer, TermWin.gc,
			       ThaiCol2Pixel(col,screen.text[screen.cur.row+TermWin.saveLines]),
			       Row2Pixel(currow),
			       Width2Pixel(1 + wbyte) - 1,
			       Height2Pixel(1) - 1);

#else
		XDrawRectangle(Xdisplay, drawBuffer, TermWin.gc,
			       Col2Pixel(col), Row2Pixel(currow),
			       Width2Pixel(1 + wbyte) - 1,
			       Height2Pixel(1) - 1);
#endif
#ifndef NO_CURSORCOLOR
		if (gcmask)	/* restore normal colours */
		    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
#endif
	    }
	}
    }


/*
 * G: cleanup selection
 */
    scr_reverse_selection();

/*
 * H: other general cleanup
 */
    if (boldlast && TermWin.bMapped)  	/* clear the whole screen height */
	XClearArea(Xdisplay, TermWin.vt, TermWin_TotalWidth() - 2, 0,
		   1, TermWin_TotalHeight() - 1, 0);
    if (type & SMOOTH_REFRESH)
	XSync(Xdisplay, False);
}
