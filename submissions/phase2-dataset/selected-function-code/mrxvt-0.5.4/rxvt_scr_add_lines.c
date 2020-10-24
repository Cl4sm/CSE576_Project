void
rxvt_scr_add_lines(rxvt_t* r, int page, const unsigned char *str, int nlines,
	int len)
{
    unsigned char   checksel, clearsel;
    char	    c;
    int		 i, row, last_col;
    text_t	 *stp;
    rend_t	 *srp;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_add_lines( r, %d, %.*s, %d, %d)\n", page, min(len, 36), str, nlines, len ));

    if (len <= 0)	/* sanity */
	return;

    PVTS(r, page)->want_refresh = 1;
    last_col = r->TermWin.ncol;

    ZERO_SCROLLBACK(r, page);
    if (nlines > 0)
    {
	/*
	 * 2006-09-02 gi1242 TODO: The code below is *horrible*. When we call
	 * rxvt_scroll_text(), we might end up with a negative CURROW. We try
	 * and be clever using this information, but rxvt_scr_gotorc() will
	 * reset this information!
	 */
	nlines += (CURROW - PSCR(r, page).bscroll);
	if (
	      (nlines > 0)
	      && (PSCR(r, page).tscroll == 0)
	      && (PSCR(r, page).bscroll == (r->TermWin.nrow - 1))
	   )
	{
	    /* _at least_ this many lines need to be scrolled */
	    rxvt_scroll_text(r, page, PSCR(r, page).tscroll,
		PSCR(r, page).bscroll, nlines, 0);
	    adjust_view_start(r, page, nlines );

	    CURROW -= nlines;

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "\e[32mScrolling %d lines. CURROW=%d\e[0m\n", nlines, CURROW ));
	}
    }

    assert(CURCOL < last_col);
    assert(CURROW < r->TermWin.nrow);

#if 0 /*{{{ Possibly incorrection assertion */
    /*
     * XXX 2006-09-12 gi1242: I think this assertion is wrong! Note that a few
     * lines later we set CURROW to be the max of CURROW and -PVTS()->nscrolled
     */
    assert(CURROW >= -(int32_t)PVTS(r, page)->nscrolled);
#endif /*}}}*/

    MIN_IT(CURCOL, last_col - 1);
    MIN_IT(CURROW, (int32_t)r->TermWin.nrow - 1);
    MAX_IT(CURROW, -(int32_t)PVTS(r, page)->nscrolled);

    row = CURROW + SVLINES;

    checksel = (SEL(r).op && SEL(r).vt == page &&
	PVTS(r, page)->current_screen == SEL(r).screen) ? 1 : 0;
    clearsel = 0;

    stp = PSCR(r, page).text[row];
    srp = PSCR(r, page).rend[row];

#ifdef MULTICHAR_SET
    if(
	 PVTS(r, page)->lost_multi && CURCOL > 0 &&
	 IS_MULTI1(srp[CURCOL - 1]) &&
	 *str != '\n' && *str != '\r' && *str != '\t'
      )
    {
	PVTS(r, page)->chstat = WBYTE;
    }
#endif

    for (i = 0; i < len;)
    {
	c = str[i++];
	switch (c)
	{
	    case '\t':
		rxvt_scr_tab(r, page, 1);
		continue;

	    case '\n':
		/* XXX: think about this */
		if( PSCR(r, page).tlen[row] != -1 )
		    MAX_IT(PSCR(r, page).tlen[row], CURCOL);

		PSCR(r, page).flags &= ~Screen_WrapNext;
		if (CURROW == PSCR(r, page).bscroll)
		{
		    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "%s:%d ",
				__FILE__, __LINE__ ));
		    rxvt_scroll_text(r, page, PSCR(r, page).tscroll,
			    PSCR(r, page).bscroll, 1, 0);
		    adjust_view_start( r, page, 1 );
		}
		else if (CURROW < (r->TermWin.nrow - 1))
		    row = (++CURROW) + SVLINES;

		stp = PSCR(r, page).text[row];  /* _must_ refresh */
		srp = PSCR(r, page).rend[row];  /* _must_ refresh */
		RESET_CHSTAT(r, page);
		continue;

	    case '\r':
		/* XXX: think about this */
		if (PSCR(r, page).tlen[row] != -1)
		    MAX_IT(PSCR(r, page).tlen[row], CURCOL);
		PSCR(r, page).flags &= ~Screen_WrapNext;
		CURCOL = 0;
		RESET_CHSTAT(r, page);
		continue;

	    default:
#ifdef MULTICHAR_SET
		if (r->encoding_method == ENC_NOENC)
		{
		    if (c == 127)
			continue;
		    break;
		}
		PVTS(r, page)->rstyle &= ~RS_multiMask;

		/* multibyte 2nd byte */
		if (PVTS(r, page)->chstat == WBYTE)
		{
		    /* set flag of second byte in style */
		    PVTS(r, page)->rstyle |= RS_multi2;
		    /* switch back to single byte for next char */
		    PVTS(r, page)->chstat = SBYTE;
		    if (
			  (r->encoding_method == ENC_EUCJ)
			  && ((char) stp[CURCOL-1] == (char) 0x8e)
		       )
		    {
			PVTS(r, page)->rstyle &= ~RS_multiMask;
			CURCOL --;
		    }
		    else
		    /* maybe overkill, but makes it selectable */
		    if ((r->encoding_method == ENC_EUCJ) ||
			(r->encoding_method == ENC_GBK) ||
			(r->encoding_method == ENC_GB))
			c |= 0x80;
		}
		/* multibyte 1st byte */
		else if (PVTS(r, page)->chstat == SBYTE)
		{
		    if (r->encoding_method == ENC_SJIS)
		    {
			if (
			      PVTS(r, page)->multi_byte
			      || (
				  (
				   (unsigned char) c >= (unsigned char) 0x81
				   && (unsigned char) c <= (unsigned char) 0x9f
				  )
				  ||
				  (
				   (unsigned char) c >= (unsigned char) 0xe0
				   && (unsigned char) c <= (unsigned char) 0xfc
				  )
				 )
			   )
			{
			    PVTS(r, page)->rstyle |= RS_multi1;
			    PVTS(r, page)->chstat = WBYTE;
			}
		    }
		    else if (PVTS(r, page)->multi_byte || (c & 0x80))
		    {
			/* set flag of first byte in style */
			PVTS(r, page)->rstyle |= RS_multi1;
			/* switch to multiple byte for next char */
			PVTS(r, page)->chstat = WBYTE;
			/* maybe overkill, but makes selectable */
			if (
			      (r->encoding_method == ENC_EUCJ)
			      || (r->encoding_method == ENC_GBK)
			      || (r->encoding_method == ENC_GB)
			   )
			    c |= 0x80;
		    }
		}
		else
#endif
		if (c == 127)
		    continue;	/* yummmm..... */
		break;
	}   /* switch */

	if (
	      checksel	    /* see if we're writing within selection */
	      && !RC_BEFORE(PSCR(r, page).cur, SEL(r).beg)
	      && RC_BEFORE(PSCR(r, page).cur, SEL(r).end)
	   )
	{
	    checksel = 0;
	    clearsel = 1;
	}

	if (PSCR(r, page).flags & Screen_WrapNext)
	{
	    PSCR(r, page).tlen[row] = -1;
	    if (CURROW == PSCR(r, page).bscroll)
	    {
		rxvt_scroll_text(r, page, PSCR(r, page).tscroll,
			PSCR(r, page).bscroll, 1, 0);
		adjust_view_start( r, page, 1 );
	    }
	    else if (CURROW < (r->TermWin.nrow - 1))
		row = (++CURROW) + SVLINES;
	    stp = PSCR(r, page).text[row];  /* _must_ refresh */
	    srp = PSCR(r, page).rend[row];  /* _must_ refresh */
	    CURCOL = 0;
	    PSCR(r, page).flags &= ~Screen_WrapNext;
	}

	if (PSCR(r, page).flags & Screen_Insert)
	    rxvt_scr_insdel_chars(r, page, 1, INSERT);

#ifdef MULTICHAR_SET
	if (
	      IS_MULTI1(PVTS(r, page)->rstyle)
	      && CURCOL > 0
	      && IS_MULTI1(srp[CURCOL - 1])
	   )
	{
	    stp[CURCOL - 1] = ' ';
	    srp[CURCOL - 1] &= ~RS_multiMask;
	}
	else if (
		  IS_MULTI2(PVTS(r, page)->rstyle)
		  && CURCOL < (last_col - 1)
		  && IS_MULTI2(srp[CURCOL + 1])
		)
	{
	    stp[CURCOL + 1] = ' ';
	    srp[CURCOL + 1] &= ~RS_multiMask;
	}
#endif

	stp[CURCOL] = c;
	srp[CURCOL] = PVTS(r, page)->rstyle;
	if (CURCOL < (last_col - 1))
	    CURCOL++;
	else
	{
	    PSCR(r, page).tlen[row] = last_col;
	    if (PSCR(r, page).flags & Screen_Autowrap)
		PSCR(r, page).flags |= Screen_WrapNext;
	}
    }	/* for */

    if (PSCR(r, page).tlen[row] != -1)	/* XXX: think about this */
	MAX_IT(PSCR(r, page).tlen[row], CURCOL);

    /*
    ** If we wrote anywhere in the selected area, kill the selection
    ** XXX: should we kill the mark too?  Possibly, but maybe that
    **	  should be a similar check.
    */
    if (clearsel)
	CLEAR_SELECTION(r);

    assert(CURROW >= 0);
    MAX_IT(CURROW, 0);
}