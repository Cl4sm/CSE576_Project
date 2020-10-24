void
rxvt_scr_add_row (rxvt_t* r, int page, unsigned int total_rows, unsigned int prev_total_rows)
{
    unsigned int    nrow, prev_nrow;
    unsigned int    p;
    register int    i;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "%s( page=%d, total_rows=%u, prev_total_rows=%u )\n", __func__, page, total_rows, prev_total_rows ));

    nrow = r->TermWin.nrow;
    prev_nrow = PVTS(r, page)->prev_nrow;

    /* add rows */
    rxvt_scr_reset_realloc(r, page);	/* realloc _first_ */

    i = min(PVTS(r, page)->nscrolled, nrow - prev_nrow);
    for (p = prev_total_rows; p < total_rows; p++)
    {
	PSCR(r, page).tlen[p] = 0;
	PSCR(r, page).text[p] = NULL;
	PSCR(r, page).rend[p] = NULL;
    }

    for (p = prev_total_rows; p < total_rows - i; p++)
	rxvt_blank_screen_mem (r, page, PSCR(r, page).text,
	    PSCR(r, page).rend, p, DEFAULT_RSTYLE);

    for (p = prev_nrow; p < nrow; p++)
    {
#if NSCREENS
	PVTS(r, page)->swap.tlen[p] = 0;
	PVTS(r, page)->swap.text[p] = NULL;
	PVTS(r, page)->swap.rend[p] = NULL;
	rxvt_blank_screen_mem (r, page, PVTS(r, page)->swap.text,
	    PVTS(r, page)->swap.rend, p, DEFAULT_RSTYLE);
#endif

	PVTS(r, page)->drawn_text[p] = NULL;
	PVTS(r, page)->drawn_rend[p] = NULL;
	rxvt_blank_screen_mem (r, page, PVTS(r, page)->drawn_text,
	    PVTS(r, page)->drawn_rend, p, DEFAULT_RSTYLE);
    }

    if (i > 0)
    {
	rxvt_scroll_text(r, page, 0, (int)nrow - 1, -i, 1);
	PSCR(r, page).cur.row += i;
	PSCR(r, page).s_cur.row += i;
	PVTS(r, page)->nscrolled -= i;
    }

    assert(PSCR(r, page).cur.row < r->TermWin.nrow);
    MIN_IT(PSCR(r, page).cur.row, nrow - 1);
#if NSCREENS
    assert(PVTS(r, page)->swap.cur.row < r->TermWin.nrow);
    MIN_IT(PVTS(r, page)->swap.cur.row, nrow - 1);
#endif
}