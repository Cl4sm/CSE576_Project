 */
/* EXTPROTO */
void
rxvt_scr_refresh(rxvt_t* r, int page, unsigned char refresh_type)
{
    unsigned char
		clearfirst, /* first character writes before cell */
		clearlast,  /* last character writes beyond cell */
		must_clear, /* use drawfunc not image_drawfunc */
		already_cleared=0, /* Use XClearArea or no-op */
#ifndef NO_BOLDFONT
		usingBoldFt, /* we've changed font to bold font */
		loadedBoldFt,	/* If a bold font is loaded */
#endif
		rvid,	    /* reverse video this position */
		wbyte,	    /* we're in multibyte */
		showcursor; /* show the cursor */


    signed char	morecur = 0;/* */
#ifdef TTY_256COLOR
    uint16_t	fore, back; /* desired foreground/background */
#else
    unsigned char
		fore, back; /* desired foreground/background */
#endif
    int16_t	col, row,   /* column/row we're processing */
		ocrow,	    /* old cursor row */
		len, wlen;  /* text length screen/buffer */
    int		i,	    /* tmp */
		row_offset; /* basic offset in screen structure */
#ifndef NO_CURSORCOLOR
    rend_t	cc1 = 0;    /* store colours at cursor position(s) */
# ifdef MULTICHAR_SET
    rend_t	cc2 = 0;    /* store colours at cursor position(s) */
			    /* 2007-07-30 gi1242: NULL assignment to suppress
			     * compile warning. */
# endif
#endif
    XGCValues	gcvalue;    /* Graphics Context values */
    XFontStruct*    wf;	    /* font structure */
    rend_t*	drp;	    /* drawn_rend pointer */
    rend_t*	srp;	    /* screen-rend-pointer */
    text_t*	dtp;	    /* drawn-text pointer */
    text_t*	stp;	    /* screen-text-pointer */
    char*	buffer;	    /* local copy of r->h->buffer */
    /*
    int		(*drawfunc) () = XDrawString;
    int		(*image_drawfunc) () = XDrawImageString;
    */
    int		drawfunc, image_drawfunc;
    struct rxvt_hidden *h = r->h;

    if( !(refresh_type & CLIPPED_REFRESH) )
	PVTS( r, page)->scrolled_lines = 0;

    if (refresh_type == NO_REFRESH || !PVTS(r, page)->mapped)
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "Skipping refresh (%d, %d)\n", refresh_type, PVTS(r, page)->mapped));

	return;
    }

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_refresh %d ()\n", page));

    /*
    ** A: set up vars
    */
#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt)
    {
	drawfunc = XFT_DRAW_STRING_8;
	image_drawfunc = XFT_DRAW_IMAGE_STRING_8;
    }
    else
#endif
    {
	drawfunc = X11_DRAW_STRING_8;
	image_drawfunc = X11_DRAW_IMAGE_STRING_8;
    }

    clearfirst = clearlast = must_clear = wbyte = 0;
#ifndef NO_BOLDFONT
    usingBoldFt = 0;

    /* Determine if we have a bold font loaded or not */
    if(
# ifdef XFT_SUPPORT
        ISSET_OPTION( r, Opt_xft )		    ?
	    NOT_NULL( r->TermWin.xftbfont )	    :
	    NOT_NULL( r->TermWin.bfont )
# else
        NOT_NULL( r->TermWin.bfont )
# endif
      )
	loadedBoldFt = 1;
    else
	loadedBoldFt = 0;
#endif /* NO_BOLDFONT */

    if (h->currmaxcol < r->TermWin.ncol)
    {
	h->currmaxcol = r->TermWin.ncol;
	h->buffer = rxvt_realloc(h->buffer, sizeof(char) * (h->currmaxcol + 1));
    }
    buffer = h->buffer;

    row_offset = SVLINES - VSTART;
#ifdef XFT_SUPPORT
    if (!(ISSET_OPTION(r, Opt_xft) && r->TermWin.xftfont))
#endif
    {
	/* always go back to the base font - it's much safer */
	XSetFont(r->Xdisplay, r->TermWin.gc, r->TermWin.font->fid);
	wf = r->TermWin.font;
    }

    if ((refresh_type & REFRESH_BOUNDS))
    {
	clearfirst = clearlast = 1;
	h->refresh_type &= ~REFRESH_BOUNDS;
    }
#ifdef BACKGROUND_IMAGE
    must_clear |= IS_PIXMAP(PVTS(r, page)->bg.pixmap);
#endif
#ifdef TRANSPARENT
    must_clear |= ( h->am_transparent || h->am_pixmap_trans );
#endif
    /* is there an old outline cursor on screen? */
    ocrow = h->oldcursor.row;

    /*
     * set base colours to avoid check in "single glyph writing" below
     */
    gcvalue.foreground = r->pixColors[Color_fg];
    gcvalue.background = r->pixColors[Color_bg];

    /*
     * Set clippings on our XftDrawables and GC's to make sure we don't waste
     * time drawing pixels outside this clipping. (This probably happened
     * because of an expose event).
     */
    if( (refresh_type & CLIPPED_REFRESH) && IS_REGION(h->refreshRegion))
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "Doing clipped refresh (Region %p)\n", h->refreshRegion));

	/*
	 * We must wait till refresh is complete before destroying the
	 * region because the clipping is reset when text shadow is used.
	 */
	XSetRegion( r->Xdisplay, r->TermWin.gc, h->refreshRegion);
#ifdef XFT_SUPPORT
	if( ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt )
	    XftDrawSetClip( PVTS(r, page)->xftvt, h->refreshRegion);
#endif
	/* Remember we don't need to call XClearArea on exposed regions */
	if( must_clear ) already_cleared = 1;
    }

    /*
    ** B: reverse any characters which are selected
    */
    rxvt_scr_reverse_selection(r, page);


    /*
    ** C: set the cursor character(s)
    */
    {
	unsigned char	setoldcursor;
	rend_t		ccol1,	/* Cursor colour */
			ccol2;	/* Cursor colour2 */

	showcursor = (PSCR(r, page).flags & Screen_VisibleCursor);
#ifdef CURSOR_BLINK
	if (h->hidden_cursor)
	    showcursor = 0;
#endif
	if (showcursor && r->TermWin.focus)
	{
	    int	    currow = CURROW + SVLINES;
	    srp = &(PSCR(r, page).rend[currow][CURCOL]);

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "Setting solid cursor\n"));

	    *srp ^= RS_RVid;

#ifndef NO_CURSORCOLOR
	    cc1 = *srp & (RS_fgMask | RS_bgMask);
	    if (XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_cursor))
		ccol1 = Color_cursor;
	    else
#ifdef CURSOR_COLOR_IS_RENDITION_COLOR
		ccol1 = GET_FGCOLOR(
			    PVTS(r, page)->drawn_rend[CURROW][CURCOL]);
		/*
		ccol1 = GET_FGCOLOR(PVTS(r, page)->rstyle);
		*/
#else
		ccol1 = Color_fg;
#endif
	    if(
		 XDEPTH > 2
		 && ISSET_PIXCOLOR( h, Color_cursor )
		 && ISSET_PIXCOLOR( h, Color_cursor2 )
	      )
		ccol2 = Color_cursor2;
	    else
	    {
#ifdef CURSOR_COLOR_IS_RENDITION_COLOR
# ifdef SIMULATE_LINUX_CONSOLE_CURSOR_COLOR
		ccol2 = GET_FGCOLOR(PVTS(r, page)->drawn_rend[CURROW][CURCOL]);
# else
		ccol2 = GET_BGCOLOR(PVTS(r, page)->drawn_rend[CURROW][CURCOL]);
# endif	/* SIMULATE_LINUX_CONSOLE_CURSOR_COLOR */
		/*
		ccol2 = GET_BGCOLOR(PVTS(r, page)->rstyle);
		*/
#else
		ccol2 = Color_bg;
#endif
	    }

	    *srp = SET_FGCOLOR(*srp, ccol1);
	    *srp = SET_BGCOLOR(*srp, ccol2);
#endif /* NO_CURSORCOLOR */

#ifdef MULTICHAR_SET
	    if (IS_MULTI1(*srp))
	    {
		if (CURCOL < r->TermWin.ncol - 2 && IS_MULTI2(*++srp))
		    morecur = 1;
	    }
	    else if (IS_MULTI2(*srp))
	    {
		if (CURCOL > 0 && IS_MULTI1(*--srp))
		    morecur = -1;
	    }
	    if (morecur)
	    {
		*srp ^= RS_RVid;
# ifndef NO_CURSORCOLOR
		cc2 = *srp & (RS_fgMask | RS_bgMask);
		*srp = SET_FGCOLOR(*srp, ccol1);
		*srp = SET_BGCOLOR(*srp, ccol2);
# endif
	    }
#endif
	}

	/* make sure no outline cursor is left around */
	setoldcursor = 0;
	if (ocrow != -1)
	{
	    if (CURROW + VSTART != ocrow || CURCOL != h->oldcursor.col)
	    {
		if (
		      ocrow < r->TermWin.nrow
		      && h->oldcursor.col < r->TermWin.ncol
		   )
		{
		    PVTS(r, page)->drawn_rend[ocrow][h->oldcursor.col] ^=
			(RS_RVid | RS_Uline);
#ifdef MULTICHAR_SET
		    if (h->oldcursormulti)
		    {
			col = h->oldcursor.col + h->oldcursormulti;
			if (col < r->TermWin.ncol)
			    PVTS(r, page)->drawn_rend[ocrow][col] ^=
				(RS_RVid | RS_Uline);
		    }
#endif
		}
		if (r->TermWin.focus || !showcursor)
		    h->oldcursor.row = -1;
		else
		    setoldcursor = 1;
	    }
	}
	else if (!r->TermWin.focus)
	    setoldcursor = 1;

	if (setoldcursor)
	{
	    if (CURROW + VSTART >= r->TermWin.nrow)
		h->oldcursor.row = -1;
	    else
	    {
		h->oldcursor.row = CURROW + VSTART;
		h->oldcursor.col = CURCOL;
#ifdef MULTICHAR_SET
		h->oldcursormulti = morecur;
#endif
	    }
	}
    }
    /* End of C */


#ifndef NO_SLOW_LINK_SUPPORT
    /*
    ** D: CopyArea pass - very useful for slower links
    **	This has been deliberately kept simple.
    */
    i = PVTS(r, page)->num_scr;
    if (refresh_type == FAST_REFRESH &&
	    h->num_scr_allow &&
	    i &&
	    abs(i) < r->TermWin.nrow &&
	    !must_clear)
    {
	int16_t	     nits;
	int	     j;
	rend_t	     *drp2;
	text_t	     *dtp2;

	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "Trying slowlink copyarea pass\n"));

	j = r->TermWin.nrow;
	wlen = len = -1;
	row = i > 0 ? 0 : j - 1;
	for (; j-- >= 0; row += (i > 0 ? 1 : -1))
	{
	    if (row + i >= 0 && row + i < r->TermWin.nrow && row + i != ocrow)
	    {
		stp = PSCR(r, page).text[row + row_offset];
		srp = PSCR(r, page).rend[row + row_offset];
		dtp = PVTS(r, page)->drawn_text[row];
		dtp2 = PVTS(r, page)->drawn_text[row + i];
		drp = PVTS(r, page)->drawn_rend[row];
		drp2 = PVTS(r, page)->drawn_rend[row + i];
		for (nits = 0, col = r->TermWin.ncol; col--; )
		    if (stp[col] != dtp2[col] ||
			srp[col] != drp2[col])
			nits--;
		    else if (stp[col] != dtp[col] ||
			srp[col] != drp[col])
			nits++;
		if (nits > 8)	    /* XXX: arbitrary choice */
		{
		    for (col = r->TermWin.ncol; col--; )
		    {
			*dtp++ = *dtp2++;
			*drp++ = *drp2++;
		    }
		    if (len == -1)
		    len = row;
		    wlen = row;
		    continue;
		}
	    }

	    if (len != -1)
	    {
		/* also comes here at end if needed because of >= above */
		if (wlen < len)
		    SWAP_IT(wlen, len, int);

		rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_refresh %d (): " "XCopyArea: %d -> %d (height: %d)\n", page, len + i, len, wlen - len + 1));
		XCopyArea(r->Xdisplay, PVTS(r, page)->vt,
		    PVTS(r, page)->vt, r->TermWin.gc,
		    0, Row2Pixel(len + i),
		    TWIN_WIDTH(r),
		    (unsigned int)Height2Pixel(wlen-len+1),
		    0, Row2Pixel(len));
		len = -1;
	    }
	}
    }	/* End of D */
#endif	/* !NO_SLOW_LINK_SUPPORT */


    /*
    ** E: main pass across every character
    */
    for (row = 0; row < r->TermWin.nrow; row++)
    {
	unsigned char	clear_next = 0;
	int		j,
			/* x offset for start of drawing (font) */
			xpixel,
			/* y offset for top of drawing */
			ypixelc;
	unsigned long	gcmask;	 /* Graphics Context mask */


	stp = PSCR(r, page).text[row + row_offset];
	srp = PSCR(r, page).rend[row + row_offset];
	dtp = PVTS(r, page)->drawn_text[row];
	drp = PVTS(r, page)->drawn_rend[row];


#ifndef NO_PIXEL_DROPPING_AVOIDANCE
	/*
	 * E1: Pixel dropping avoidance.  Do this before the main refresh on the
	 * line. Require a refresh where pixels may have been dropped into our
	 * area by a neighbour character which has now changed
	 *
	 * TODO: This could be integrated into E2 but might be too messy
	 */
	for (col = 0; col < r->TermWin.ncol; col++)
	{
	    unsigned char   is_font_char, is_same_char;
	    text_t	    t;

	    t = dtp[col];
	    is_same_char = (t == stp[col] && drp[col] == srp[col]);
	    if (!clear_next &&
		(is_same_char || t == 0 || t == ' '))
		/* screen cleared elsewhere */
		continue;

	    if (clear_next)
	    {
		/* previous char caused change here */
		clear_next = 0;
		dtp[col] = 0;

		/* don't cascade into next char */
		if (is_same_char)
		    continue;
	    }

#ifdef XFT_SUPPORT
	    if( ISSET_OPTION(r, Opt_xft) )
	    {
		/*
		 * Only text drawn by over striking needs to be watched for
		 * "dropped pixels".
		 * 
		 * XXX This does not take into account Color_BD
		 */
		if( !loadedBoldFt && ( drp[col] & RS_Bold ) )
		{
		    if( col == r->TermWin.ncol - 1 ) clearlast = 1;
		    else clear_next = 1;
		}
# ifdef TEXT_SHADOW
		/*
		 * Xft with shadow drops pixels into the next / prev char.
		 */
		else if (
			  h->rs[Rs_textShadow]
			  && r->TermWin.shadow_mode != SHADOW_NONE
			)
		{
		    switch( r->TermWin.shadow_mode )
		    {
			case SHADOW_TOPRIGHT:
			case SHADOW_RIGHT:
			case SHADOW_BOTRIGHT:
			    /* Clear next char */
			    if( col == r->TermWin.ncol - 1) clearlast = 1;
			    else clear_next = 1;
			    break;

			case SHADOW_TOPLEFT:
			case SHADOW_LEFT:
			case SHADOW_BOTLEFT:
			    /* Clear prev char */
			    if( col == 0 ) clearfirst = 1;
			    else dtp[col-1] = 0;
			    break;

			default:
			    /* Nothing to be done here */
			    break;
		    }
		}
# endif	/* TEXT_SHADOW */
	    }
	    else
#endif
	    {
		j = MONO_BOLD(drp[col]) ? 1 : 0;
# ifndef NO_BOLDFONT
		wf = (j && r->TermWin.bfont) ?
			    r->TermWin.bfont : r->TermWin.font;
# endif

		/*
		** TODO: consider if anything special needs to happen
		** with:
		** #if defined(MULTICHAR_SET) &&
		** !defined(NO_BOLDOVERSTRIKE_MULTI)
		*/
		is_font_char = (wf->per_char && IS_FONT_CHAR(wf, t)) ? 1:0;
		if (!is_font_char || FONT_LBEAR(wf, t) < 0)
		{
		    if (col == 0)
			clearfirst = 1;
		    else
			dtp[col - 1] = 0;
		}
		if (!is_font_char ||
			(FONT_WIDTH(wf, t) < (FONT_RBEAR(wf, t) + j)))
		{
		    if (col == r->TermWin.ncol - 1)
			clearlast = 1;
		    else
			clear_next = 1;
		}
	    }
	}
#endif	/* NO_PIXEL_DROPPING_AVOIDANCE */
	/* End of E1 */


	/*
	** E2: OK, now the real pass
	*/
	ypixelc = (int)Row2Pixel(row);

	for (col = 0; col < r->TermWin.ncol; col++)
	{
			    /* current font size != base font size */
	    unsigned char   fontdiff,
			    /* proportional font used */
			    fprop;
			    /* rendition value */
	    rend_t	    rend;

	    /* screen rendition (target rendtion) */
	    rend = srp[col];

	    /*
	     * compare new text with old - if exactly the same then continue
	     */
	    if (
		 /* Must match characters to skip. */
		 stp[col] == dtp[col] &&
		 /* Either rendition the same or   */
		 (
		   rend == drp[col] ||
		   /* space w/ no background change  */
		   (
		     stp[col] == ' ' &&
		     	 GET_BGATTR(rend) == GET_BGATTR(drp[col])
		   )
		 )
	       )    /* if */
	    {
		if (!IS_MULTI1(rend))
		    continue;
#ifdef MULTICHAR_SET
		else
		{
		    /* first byte is Kanji so compare second bytes */
		    if (stp[col + 1] == dtp[col + 1])
		    {
			/* assume no corrupt characters on the screen */
			col++;
			continue;
		    }
		}
#endif
	    }
	    /* redraw one or more characters */

	    fontdiff = 0;
	    len = 0;
	    buffer[len++] = dtp[col] = stp[col];
	    drp[col] = rend;
	    xpixel = Col2Pixel(col);

	    /*
	     * Find out the longest string we can write out at once
	     */
#ifndef NO_BOLDFONT
	    if (MONO_BOLD(rend) && r->TermWin.bfont != NULL)
		fprop = (r->TermWin.propfont & PROPFONT_BOLD);
	    else
#endif
		fprop = (r->TermWin.propfont & PROPFONT_NORMAL);

#ifdef MULTICHAR_SET
	    if (
		  IS_MULTI1(rend) && col < r->TermWin.ncol - 1
		  && IS_MULTI2(srp[col + 1])
	       )
	    {
		if (!wbyte && r->TermWin.mfont)
		{
		    wbyte = 1;
		    XSetFont(r->Xdisplay, r->TermWin.gc,
			 r->TermWin.mfont->fid);
		    fontdiff = (r->TermWin.propfont & PROPFONT_MULTI);
#ifdef XFT_SUPPORT
		    if ( ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt )
		    {
			drawfunc = XFT_DRAW_STRING_16;
			image_drawfunc = XFT_DRAW_IMAGE_STRING_16;
		    }
		    else
#endif
		    {
			drawfunc = X11_DRAW_STRING_16;
			image_drawfunc = X11_DRAW_IMAGE_STRING_16;
		    }
		}

		if (r->TermWin.mfont == NULL)
		{
		    buffer[0] = buffer[1] = ' ';
		    len = 2;
		    col++;
		}
		else
		{
		    /* double stepping - we're in multibyte font mode */
		    for (; ++col < r->TermWin.ncol;)
		    {
			/* XXX: could check sanity on 2nd byte */
			dtp[col] = stp[col];
			drp[col] = srp[col];
			buffer[len++] = stp[col];
			col++;
			/* proportional multibyte font mode */
			if (fprop)
			    break;
			if ((col == r->TermWin.ncol) ||
			    (srp[col] != rend))
			    break;
			if ((stp[col] == dtp[col]) &&
			    (srp[col] == drp[col]) &&
			    (stp[col + 1] == dtp[col + 1]))
			    break;
			if (len == h->currmaxcol)
			    break;
			dtp[col] = stp[col];
			drp[col] = srp[col];
			buffer[len++] = stp[col];
		    }
		    col--;
		}

		if (buffer[0] & 0x80)
		    (h->multichar_decode)( (unsigned char*) buffer, len);
		wlen = len / 2;
	    }
	    else
	    {
		if (rend & RS_multi1)
		{
		    /* corrupt character - you're outta there */
		    rend &= ~RS_multiMask;
		    drp[col] = rend; /* TODO check: may also want */
		    dtp[col] = ' '; /* to poke into stp/srp   */
		    buffer[0] = ' ';
		}
		if (wbyte)
		{
		    wbyte = 0;
#ifdef XFT_SUPPORT
		    if (!(ISSET_OPTION(r, Opt_xft) && r->TermWin.xftfont))
#endif
		    XSetFont(r->Xdisplay, r->TermWin.gc,
			r->TermWin.font->fid);
#ifdef XFT_SUPPORT
		    if ( ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt )
		    {
			drawfunc = XFT_DRAW_STRING_8;
			image_drawfunc = XFT_DRAW_IMAGE_STRING_8;
		    }
		    else
#endif
		    {
			drawfunc = X11_DRAW_STRING_8;
			image_drawfunc = X11_DRAW_IMAGE_STRING_8;
		    }
		}   /* if (wbyte) */
#else
	    { /* add } for correct % bouncing */
#endif
		if (!fprop)
		{
		    int echars;

		    /* single stepping - `normal' mode */
		    for (i = 0, echars=0; ++col < r->TermWin.ncol - 1;)
		    {
			/*
			 * Instead of simply getting the longest string that
			 * needs to be refreshed, we do some caching.
			 *
			 * i is the number of trailing chars that we read (in an
			 * attempt to cache) that DO NOT need to be refreshed.
			 * These had better be dumped.
			 *
			 * echars are the number of extra chars we drew that did
			 * not need to be drawn. When echars get's too high,
			 * then we should break out.
			 */
			if (rend != srp[col])
			    /* Different attributes. */
			    break;
			buffer[len++] = stp[col];

			if ( (stp[col] != dtp[col]) || (srp[col] != drp[col]) )
			{
			    /* This position needed to be refreshed anyway */
			    /* if (must_clear && (i++ > (len / 2))) break; */

			    dtp[col] = stp[col];
			    drp[col] = srp[col];
			    i = 0; /* Set trailing chars to 0 */
			}
			else /* if (must_clear ||
				(stp[col] != ' ' && ++i > 32)) */
			{
			    /*
			     * This position did not require a refresh. Let's do
			     * some caching.
			     */
			    i++;
			    /*
			     * 25% (arbitarily choosen) of our drawn string can
			     * be extra chars.
			     */
			    if( ++echars > (len >> 2) ) break;
			}

		    }	/* for */
		    col--;	/* went one too far.  move back */
		    len -= i;	/* dump any matching trailing chars */

		    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "Drawing %d(%d) chars: %.*s\n", len, echars-i, (len > 55) ? 55 : len, buffer));
		} /* if (!fprop) */
		wlen = len;
	    }
	    buffer[len] = '\0';

	    /*
	     * Determine the attributes for the string
	     */
	    fore = GET_FGCOLOR(rend);
	    back = GET_BGCOLOR(rend);
	    rend = GET_ATTR(rend);

	    switch (rend & RS_fontMask)
	    {
		case RS_acsFont:
		    for (i = 0; i < len; i++)
			/*
			 * Xterm leaves 0x5f (_) unchanged in graphics mode.
			 */
			if (buffer[i] > 0x5f && buffer[i] < 0x7f)
			    buffer[i] -= 0x5f;
		    break;
		case RS_ukFont:
		    for (i = 0; i < len; i++)
			if (buffer[i] == '#')
			    buffer[i] = 0x1e;	/* pound sign */
		    break;
	    }

#ifndef NO_BOLD_UNDERLINE_REVERSE
	    /*
	     * Bold / underline fonts. We use Color_BD / Color_UL only if we're
	     * displaying the font with normal colors.
	     */
	    if( fore == Color_fg && back == Color_bg )
	    {
		/*
		 * TODO: Should probably add a Color_BDUL here.
		 */
		if ( rend & RS_Bold )
		{
		    if (
			  XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_BD)
			  && r->pixColors[fore] != r->pixColors[Color_BD]
			  && r->pixColors[back] != r->pixColors[Color_BD]
		       )
		    {

			fore = Color_BD;
			/*
			 * 2006-03-27 gi1242: Ignore veryBold when Color_BD is
			 * set. veryBold will only be used when displaying
			 * colored / blinking text.
			 */
#if 0
			if (NOTSET_OPTION(r, Opt2_veryBold))
#endif
			rend &= ~RS_Bold;   /* we've taken care of it */
		    }
		}
		else if ( rend & RS_Uline )
		{
		    if (
			  XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_UL)
			  && r->pixColors[fore] != r->pixColors[Color_UL]
			  && r->pixColors[back] != r->pixColors[Color_UL]
		       )
		    {
			fore = Color_UL;
			rend &= ~RS_Uline;  /* we've taken care of it */
		    }
		}
	    }
#endif
	    rvid = (rend & RS_RVid) ? 1 : 0;
#ifdef OPTION_HC
	    /*
	     * Use Color_HC for anything blinking. TODO: Add a seperate
	     * attribute for the XSelection, so that the user can distinguish
	     * blinking text from the selection. (XTerm does this)
	     */
	    if ( (rend & RS_Blink))
	    {
		if (
		      XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_HC)
		      && r->pixColors[fore] != r->pixColors[Color_HC]
		      && r->pixColors[back] != r->pixColors[Color_HC]
# ifndef NO_CURSORCOLOR
			/* Don't do this for the cursor */
		      && (
			    !ISSET_PIXCOLOR(h, Color_cursor)
			    || !r->TermWin.focus || !showcursor
			    || CURROW != row || CURCOL != col
			 )
# endif
		   )
		{
		    if( rvid) rvid = 0;
		    back = Color_HC;
		}
		else
		    rvid = 1;	/* fall back */
	    }
#endif

	    /*
	     * Reverse Video. If defined, Color_RV for background and leave
	     * foreground untouched. Done last so that RV-BD text will have
	     * Color_BD background if set (like in XTerm).
	     */
	    if( rvid )
	    {
#ifndef NO_BOLD_UNDERLINE_REVERSE
		if (
		      XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_RV)
		      && r->pixColors[fore] != r->pixColors[Color_RV]
		      && r->pixColors[back] != r->pixColors[Color_RV]
# ifndef NO_CURSORCOLOR
		      /* Don't do this for the cursor */
		      && (
			    !ISSET_PIXCOLOR(h, Color_cursor)
			    || !r->TermWin.focus || !showcursor
			    || CURROW != row || CURCOL != col
			 )
# endif
		   )
		    back = Color_RV;
		else
#endif
		{
#ifdef TTY_256COLOR
		    SWAP_IT(fore, back, uint16_t);
#else
		    SWAP_IT(fore, back, unsigned char);
#endif
		}
	    }

#ifndef NO_BRIGHTCOLOR
	    /* Use bright colors for bold primary colors */
	    if( (rend & RS_Bold) && NOTSET_OPTION( r, Opt2_boldColors ) )
	    {
		if( fore >= minCOLOR && fore < minBrightCOLOR )
		{
		    fore += minBrightCOLOR - minCOLOR;
		    if( NOTSET_OPTION( r, Opt_veryBright ) )
			rend &= ~RS_Bold;
		}
#if defined(TTY_256COLOR) && defined(BOLD_BRIGHTENS_256_COLORS)
		/* If fore is in the 6x6x6 color cube, try and brighten it */
		else if(
			 fore >= min256COLOR			    &&
			 fore <= min256COLOR + 4*36 + 4*6 + 4	    &&
			 (fore - min256COLOR) % 6 < 5		    &&
			 ((fore - min256COLOR)/6) % 6 < 5
		       )
		{
		    fore += 36 + 6 + 1;

		    if( NOTSET_OPTION( r, Opt_veryBright ) )
			rend &= ~RS_Bold;
		}

		/* Brighten up colors in the grey-scale ramp. */
		else if(
			fore >= min256COLOR + 6*6*6		    &&
			fore <= max256COLOR - 3
		       )
		{
		    if( fore == max256COLOR -3 )
			fore = min256COLOR + 6*6*6 - 1;
		    else
			fore += 4;

		    if( NOTSET_OPTION( r, Opt_veryBright ) )
			rend &= ~RS_Bold;
		}
#endif /*TTY_256COLOR && BOLD_BRIGHTENS_256_COLORS*/
	    }
#endif /*NO_BRIGHTCOLOR*/


	    /*
	     * fore and back should now have the correct colors.
	     */
	    gcmask = 0;
	    if (back != Color_bg)
	    {
		gcvalue.background = r->pixColors[back];
		gcmask = GCBackground;
	    }
	    if (fore != Color_fg)
	    {
		gcvalue.foreground = r->pixColors[fore];
		gcmask |= GCForeground;
	    }
#ifndef NO_BOLD_UNDERLINE_REVERSE
	    else if (rend & RS_Bold && ISSET_PIXCOLOR( h, Color_BD) )
	    {
# ifdef XFT_SUPPORT
		/*
		 * XFT won't use the colors from the GC, so we need to set
		 * fore.
		 */
		if( ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt )
		    fore = Color_BD;
		else
# endif
		{
		    gcvalue.foreground = r->pixColors[Color_BD];
		    gcmask |= GCForeground;
		}

		/*
		 * If veryBold is not set, then don't render colored text in
		 * bold.
		 */
		if (NOTSET_OPTION(r, Opt2_veryBold))
		    rend &= ~RS_Bold;
	    }
	    else if (rend & RS_Uline && ISSET_PIXCOLOR( h, Color_UL) )
	    {
# ifdef XFT_SUPPORT
		if( ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt )
		    fore = Color_UL;
		else
# endif
		{
		    gcvalue.foreground = r->pixColors[Color_UL];
		    gcmask |= GCForeground;
		}
		rend &= ~RS_Uline;  /* we've taken care of it */
	    }
#endif


	    if (gcmask)
		XChangeGC(r->Xdisplay, r->TermWin.gc, gcmask, &gcvalue);
#ifndef NO_BOLDFONT
	    /*
	     * Switch to the bold font if we are rendering bold text.
	     *
	     * NOTE: We only deal with bold fonts for non-multichar text.
	     * Multichar bold text will have to be done by over striking (or
	     * some other shmuck must code it) -- gi1242 2006-08-19.
	     */
	    if ( MONO_BOLD_FG(rend, fore) && !wbyte )
	    {
		if( usingBoldFt )
		    rend &= ~RS_Bold;	/* We've taken care of it */

		else if( loadedBoldFt )
		{
		    usingBoldFt = 1;

# ifdef XFT_SUPPORT
		    if( ISSET_OPTION(r, Opt_xft) )
		    {
			SWAP_IT( r->TermWin.xftfont, r->TermWin.xftbfont,
				XftFont*);
		    }
		    else
# endif
		    {
			XSetFont(r->Xdisplay, r->TermWin.gc,
				r->TermWin.bfont->fid);
		    }

		    fontdiff = (r->TermWin.propfont & PROPFONT_BOLD);
		    rend &= ~RS_Bold;   /* we've taken care of it */
		}
	    }

	    /*
	     * If we are using the bold font, but don't want to render bold
	     * text, then we should restore the original font.
	     */
	    else if( usingBoldFt && !MONO_BOLD_FG( rend, fore ) )
	    {
		usingBoldFt = 0;

		/*
		 * If we're not showing a multi byte char, then we reset
		 * fontdiff to 0. If we're showing a multi byte char, then font
		 * diff will have been set elsewhere, and we should not reset
		 * it.
		 */
		if( !wbyte )
		    fontdiff    = 0;

# ifdef XFT_SUPPORT
		if( ISSET_OPTION(r, Opt_xft) )
		{
		    SWAP_IT( r->TermWin.xftfont, r->TermWin.xftbfont, XftFont*);
		}
		else
# endif
		{
		    XSetFont(r->Xdisplay, r->TermWin.gc, r->TermWin.font->fid);
		}
	    }
#endif

	    /*
	     * Actually do the drawing of the string here
	     */
	    if (back == Color_bg && must_clear)
	    {
		CLEAR_CHARS( r, page, already_cleared,
			xpixel, ypixelc, len);
		for (i = 0; i < len; i++)
		    /* don't draw empty strings */
		    if (buffer[i] != ' ')
		    {
			rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "CL Drawing '%.60s' (%d)\n", buffer, len));

			rxvt_scr_draw_string (r, page, xpixel, ypixelc,
				buffer, wlen, drawfunc,
				fore, back, rend,
				((refresh_type & CLIPPED_REFRESH) ?
					r->h->refreshRegion : None ));
			break;
		    }
	    }
	    else if (fprop || fontdiff)
	    {
		/* single glyph writing */
		unsigned long   pixel;

		pixel = gcvalue.foreground;
		gcvalue.foreground = gcvalue.background;
		XChangeGC(r->Xdisplay, r->TermWin.gc, GCForeground, &gcvalue);
		rxvt_fill_rectangle (r, page, xpixel, ypixelc,
		    (unsigned int) Width2Pixel(len),
		    (unsigned int) (Height2Pixel(1)
		    /* - r->TermWin.lineSpace */));
		gcvalue.foreground = pixel;
		XChangeGC(r->Xdisplay, r->TermWin.gc, GCForeground, &gcvalue);

		rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "PF Drawing '%.60s' (%d)\n", buffer, len));
		rxvt_scr_draw_string (r, page,
			xpixel, ypixelc, buffer, wlen, drawfunc,
			fore, back, rend,
			((refresh_type & CLIPPED_REFRESH) ?
				r->h->refreshRegion : None ));
	    }
	    else
	    {
		rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "NC Drawing '%.60s' (%d)\n", buffer, len));
		rxvt_scr_draw_string (r, page,
			xpixel, ypixelc, buffer, wlen, image_drawfunc,
			fore, back, rend,
			((refresh_type & CLIPPED_REFRESH) ?
				r->h->refreshRegion : None ));
	    }

#ifndef NO_BOLDOVERSTRIKE
# ifdef NO_BOLDOVERSTRIKE_MULTI
	    if (!wbyte)
# endif
	    if (MONO_BOLD_FG(rend, fore))
	    {
		/*
		 * If we still need to draw a bold chars, then all else has
		 * failed. Fall back to overstriking.
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "Overstriking %s\n", buffer ));
		rxvt_scr_draw_string (r, page,
			xpixel + 1, ypixelc, buffer, wlen, drawfunc,
			fore, back, rend,
			((refresh_type & CLIPPED_REFRESH) ?
				r->h->refreshRegion : None ));
	    }
#endif
	    if (rend & RS_Uline)
	    {
#ifdef XFT_SUPPORT
		if (ISSET_OPTION(r, Opt_xft) && PVTS(r, page)->xftvt)
		{
		    if (r->TermWin.xftfont->descent > 1)
			XDrawLine(r->Xdisplay, drawBuffer,
			    r->TermWin.gc,
			    xpixel,
			    ypixelc + r->TermWin.xftfont->ascent + 1,
			    xpixel + Width2Pixel(len) - 1,
			    ypixelc + r->TermWin.xftfont->ascent + 1);
		}
		else
#endif
		if (r->TermWin.font->descent > 1)
		    XDrawLine(r->Xdisplay, drawBuffer, r->TermWin.gc,
			xpixel,
			ypixelc + r->TermWin.font->ascent + 1,
			xpixel + Width2Pixel(len) - 1,
			ypixelc + r->TermWin.font->ascent + 1);
	    }

	    if (gcmask)	    /* restore normal colours */
	    {
		gcvalue.foreground = r->pixColors[Color_fg];
		gcvalue.background = r->pixColors[Color_bg];
		XChangeGC(r->Xdisplay, r->TermWin.gc, gcmask, &gcvalue);
	    }
	} /* for (col....) */

	/* End of E2 */
    } /* for (row....) */

    /*
     * If we've completed our refresh, and are using the bold font, we need to
     * reset it. Only needed when using XFT.
     */
    if( usingBoldFt )
    {
	usingBoldFt = 0;

#ifdef XFT_SUPPORT
	if( ISSET_OPTION(r, Opt_xft) )
	{
	    SWAP_IT( r->TermWin.xftfont, r->TermWin.xftbfont, XftFont*);
	}
	else
# endif
	{
	    XSetFont(r->Xdisplay, r->TermWin.gc, r->TermWin.font->fid);
	}
    }
    /* End of E */


    /*
    ** G: cleanup cursor and display outline cursor in necessary
    */
    if (showcursor)
    {
	if (r->TermWin.focus)
	{
	    int	    currow = CURROW + SVLINES;
	    srp = &(PSCR(r, page).rend[currow][CURCOL]);
	    *srp ^= RS_RVid;

#ifndef NO_CURSORCOLOR
	    *srp = (*srp & ~(RS_fgMask | RS_bgMask)) | cc1;
#endif
#ifdef MULTICHAR_SET
	    if (morecur)
	    {
		assert (0 == morecur || -1 == morecur || 1 == morecur);
		srp += morecur;
		*srp ^= RS_RVid;
# ifndef NO_CURSORCOLOR
		*srp = (*srp & ~(RS_fgMask | RS_bgMask)) | cc2;
# endif
	    }
#endif
	}
	else if (h->oldcursor.row >= 0)
	{
#ifndef NO_CURSORCOLOR
	    unsigned long   gcmask; /* Graphics Context mask */

	    gcmask = 0;
	    if (XDEPTH > 2 && ISSET_PIXCOLOR(h, Color_cursor))
	    {
		gcvalue.foreground = r->pixColors[Color_cursor];
		gcmask = GCForeground;
		XChangeGC(r->Xdisplay, r->TermWin.gc, gcmask, &gcvalue);
		    gcvalue.foreground = r->pixColors[Color_fg];
	    }
#endif

	    XDrawRectangle(r->Xdisplay, drawBuffer, r->TermWin.gc,
		Col2Pixel(h->oldcursor.col + morecur),
		Row2Pixel(h->oldcursor.row),
		(unsigned int)(Width2Pixel(1 + (morecur?1:0)) - 1),
		(unsigned int)(Height2Pixel(1)
		/* - r->TermWin.lineSpace*/ - 1));

#ifndef NO_CURSORCOLOR
	    if (gcmask)	    /* restore normal colours */
		XChangeGC(r->Xdisplay, r->TermWin.gc, gcmask, &gcvalue);
#endif
	}
    }
    /* End of G */


    /*
    ** H: cleanup selection
    */
    rxvt_scr_reverse_selection(r, page);


    /*
    ** I: other general cleanup
    */
    /* 
    ** clear the whole screen height, note that width == 0 is treated
    ** specially by XClearArea
    */
    if (clearfirst && r->TermWin.int_bwidth)
	rxvt_clear_area (r, page, 0, 0,
	    (unsigned int)r->TermWin.int_bwidth, VT_HEIGHT(r));
    /* 
    ** clear the whole screen height, note that width == 0 is treated
    ** specially by XClearArea
    */
    if (clearlast && r->TermWin.int_bwidth)
	rxvt_clear_area (r, page,
	    TWIN_WIDTH(r) + r->TermWin.int_bwidth, 0,
	    (unsigned int)r->TermWin.int_bwidth, VT_HEIGHT(r));

    if (refresh_type & SMOOTH_REFRESH)
	XSync(r->Xdisplay, False);

    if( (refresh_type & CLIPPED_REFRESH) && IS_REGION(h->refreshRegion))
    {
	/*
	 * A clipped refresh is complete. Don't restrict future refreshes.
	 */
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "Completed clipped refresh\n"));

	/*
	 * XSetRegion( r->Xdisplay, r->TermWin.gc, None) causes a segfault.
	 * Probably because GC's dont' accept the None region as gracefully.
	 */
	XSetClipMask( r->Xdisplay, r->TermWin.gc, None);
#ifdef XFT_SUPPORT
	if( ISSET_OPTION(r, Opt_xft) && PVTS( r, page)->xftvt)
	    XftDrawSetClip( PVTS( r, page)->xftvt, None);
#endif
    }
    else
	/* If we performed an unclipped refresh, then the screen is current */
	PVTS(r, page)->want_refresh = 0;

    h->refresh_type &= ~CLIPPED_REFRESH;
    h->want_clip_refresh = 0; /* clipping is current (regardless of wether we
				 performed a clipped refresh or not. */

    /* Clipping regions will now carry stale information. */
    if (IS_REGION(h->refreshRegion))
    {
	XDestroyRegion( h->refreshRegion );
	UNSET_REGION(h->refreshRegion);
    }

    PVTS(r, page)->num_scr = 0;