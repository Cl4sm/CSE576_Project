void
rxvt_scr_gotorc(rxvt_t* r, int page, int row, int col, int relative)
{
    PVTS(r, page)->want_refresh = 1;
    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_gotorc %d (r:%s%d,c:%s%d): from (r:%d,c:%d)\n", page, (relative & R_RELATIVE ? "+" : ""), row, (relative & C_RELATIVE ? "+" : ""), col, CURROW, CURCOL));

    CURCOL = ((relative & C_RELATIVE) ? (CURCOL + col)
			 : col);
    MAX_IT(CURCOL, 0);
    MIN_IT(CURCOL, (int32_t)r->TermWin.ncol - 1);

    PSCR(r, page).flags &= ~Screen_WrapNext;
    if (relative & R_RELATIVE)
    {
	if (row > 0)
	{
	    if (CURROW <= PSCR(r, page).bscroll &&
		(CURROW + row) > PSCR(r, page).bscroll)
		CURROW = PSCR(r, page).bscroll;
	    else
		CURROW += row;
	}

	else if (row < 0)
	{
	    if (CURROW >= PSCR(r, page).tscroll &&
		(CURROW + row) < PSCR(r, page).tscroll)
		CURROW = PSCR(r, page).tscroll;
	    else
		CURROW += row;
	}
    }
    else
    {
	if (PSCR(r, page).flags & Screen_Relative)
	{
	    /* relative origin mode */
	    CURROW = row + PSCR(r, page).tscroll;
	    MIN_IT(CURROW, PSCR(r, page).bscroll);
	}
	else
	    CURROW = row;
    }
    MAX_IT(CURROW, 0);
    MIN_IT(CURROW, (int32_t)r->TermWin.nrow - 1);
}