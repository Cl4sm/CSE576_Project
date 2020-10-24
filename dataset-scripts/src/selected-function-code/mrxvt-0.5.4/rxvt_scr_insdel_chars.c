void
rxvt_scr_insdel_chars(rxvt_t* r, int page, int count, int insdel)
{
    int		col, row;
    rend_t	tr;
    text_t*	stp;
    rend_t*	srp;
    int16_t*	slp;

    PVTS(r, page)->want_refresh = 1;
    ZERO_SCROLLBACK(r, page);
#if 0
    RESET_CHSTAT(r, page);
#endif
    if (count <= 0)
	return;

    rxvt_selection_check(r, page, 1);
    MIN_IT(count, (r->TermWin.ncol - CURCOL));

    row = CURROW + SVLINES;
    PSCR(r, page).flags &= ~Screen_WrapNext;

    stp = PSCR(r, page).text[row];
    srp = PSCR(r, page).rend[row];
    slp = &(PSCR(r, page).tlen[row]);
    switch (insdel)
    {
	case INSERT:
	    for (col = r->TermWin.ncol - 1; (col - count) >= CURCOL; col--)
	    {
		stp[col] = stp[col - count];
		srp[col] = srp[col - count];
	    }
	    if (*slp != -1)
	    {
		*slp += count;
		MIN_IT(*slp, r->TermWin.ncol);
	    }
	    if (
		  SEL(r).op && SEL(r).vt == page
		  && PVTS(r, page)->current_screen == SEL(r).screen
		  && RC_ROW_ATAFTER(SEL(r).beg, PSCR(r, page).cur)
	       )
	    {
		if (
		      SEL(r).end.row != CURROW
		      || (SEL(r).end.col + count >= r->TermWin.ncol)
		   )
		    CLEAR_SELECTION(r);
		else	    /* shift selection */
		{
		    SEL(r).beg.col += count;
		    SEL(r).mark.col += count;	/* XXX: yes? */
		    SEL(r).end.col += count;
		}
	    }
	    rxvt_blank_line(&(stp[CURCOL]),
		&(srp[CURCOL]),
		(unsigned int)count, PVTS(r, page)->rstyle);
	    break;

	case ERASE:
	    CURCOL += count;	/* don't worry if > r->TermWin.ncol */
	    rxvt_selection_check(r, page, 1);
	    CURCOL -= count;
	    rxvt_blank_line(&(stp[CURCOL]),
		&(srp[CURCOL]),
		(unsigned int)count, PVTS(r, page)->rstyle);
	    break;

	case DELETE:
	    tr = srp[r->TermWin.ncol - 1]
		 & (RS_fgMask | RS_bgMask | RS_baseattrMask);
	    for (col = CURCOL; (col + count) < r->TermWin.ncol; col++)
	    {
		stp[col] = stp[col + count];
		srp[col] = srp[col + count];
	    }
	    rxvt_blank_line(&(stp[r->TermWin.ncol - count]),
		&(srp[r->TermWin.ncol - count]),
		(unsigned int)count, tr);
	    if (*slp == -1) /* break line continuation */
		*slp = r->TermWin.ncol;
	    *slp -= count;
	    MAX_IT(*slp, 0);
	    if (
		  SEL(r).op && SEL(r).vt == page
		  && PVTS(r, page)->current_screen == SEL(r).screen
		  && RC_ROW_ATAFTER(SEL(r).beg, PSCR(r, page).cur)
	       )
	    {
		if (
		      SEL(r).end.row != CURROW
		      || (CURCOL >= SEL(r).beg.col - count)
		      || SEL(r).end.col >= r->TermWin.ncol
		   )
		    CLEAR_SELECTION(r);
		else
		{
		    /* shift selection */
		    SEL(r).beg.col -= count;
		    SEL(r).mark.col -= count;	/* XXX: yes? */
		    SEL(r).end.col -= count;
		}
	    }
	    break;
    }
#if 0
    if (IS_MULTI2(srp[0]))
    {
	srp[0] &= ~RS_multiMask;
	stp[0] = ' ';
    }
    if (IS_MULTI1(srp[r->TermWin.ncol - 1]))
    {
	srp[r->TermWin.ncol - 1] &= ~RS_multiMask;
	stp[r->TermWin.ncol - 1] = ' ';
    }
#endif
}