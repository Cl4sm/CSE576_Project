void
rxvt_scr_erase_line(rxvt_t* r, int page, int mode)
{
    unsigned int    row, col, num;

    PVTS(r, page)->want_refresh = 1;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_erase_line %d (%d) at screen row: %d\n", page, mode, CURROW));
    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);
    rxvt_selection_check(r, page, 1);

    PSCR(r, page).flags &= ~Screen_WrapNext;

    row = SVLINES + CURROW;
    switch (mode)
    {
	case 0:		/* erase to end of line */
	    col = CURCOL;
	    num = r->TermWin.ncol - col;
	    MIN_IT(PSCR(r, page).tlen[row], (int16_t)col);
	    if (
		  RC_ROW_ATAFTER(SEL(r).beg, PSCR(r, page).cur)
		  || RC_ROW_ATAFTER(SEL(r).end, PSCR(r, page).cur)
	       )
		CLEAR_SELECTION(r);
	    break;
	case 1:		/* erase to beginning of line */
	    col = 0;
	    num = CURCOL + 1;
	    if (
		  RC_ROW_ATBEFORE(SEL(r).beg, PSCR(r, page).cur)
		  || RC_ROW_ATBEFORE(SEL(r).end, PSCR(r, page).cur)
	       )
		CLEAR_SELECTION(r);
	    break;
	case 2:		/* erase whole line */
	    col = 0;
	    num = r->TermWin.ncol;
	    PSCR(r, page).tlen[row] = 0;
	    if (SEL(r).beg.row <= CURROW && SEL(r).end.row >= CURROW)
		CLEAR_SELECTION(r);
	    break;
	default:
	    return;
    }

    if (PSCR(r, page).text[row])
	rxvt_blank_line(&(PSCR(r, page).text[row][col]),
	    &(PSCR(r, page).rend[row][col]), num, PVTS(r, page)->rstyle);
    else
	rxvt_blank_screen_mem(r, page, PSCR(r, page).text,
	    PSCR(r, page).rend, row, PVTS(r, page)->rstyle);
}