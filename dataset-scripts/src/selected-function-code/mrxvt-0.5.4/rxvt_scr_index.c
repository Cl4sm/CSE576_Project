void
rxvt_scr_index(rxvt_t* r, int page, enum page_dirn direction)
{
    int		 dirn;

    PVTS(r, page)->want_refresh = 1;
    dirn = ((direction == UP) ? 1 : -1);
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_index %d (%d)\n", page, dirn));

    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);

    PSCR(r, page).flags &= ~Screen_WrapNext;
    if ((CURROW == PSCR(r, page).bscroll && direction == UP) ||
	(CURROW == PSCR(r, page).tscroll && direction == DN))
	rxvt_scroll_text(r, page, PSCR(r, page).tscroll,
		PSCR(r, page).bscroll, dirn, 0);
    else
	CURROW += dirn;
    MAX_IT(CURROW, 0);
    MIN_IT(CURROW, (int32_t)r->TermWin.nrow - 1);
    rxvt_selection_check(r, page, 0);
}