void
rxvt_scr_adjust_col (rxvt_t* r, int page, unsigned int total_rows)
{
    unsigned int    nrow, ncol, prev_ncol;
    unsigned int    p;


    nrow = r->TermWin.nrow;
    ncol = r->TermWin.ncol;
    prev_ncol = PVTS(r, page)->prev_ncol;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "%s( r, page=%d, total_rows=%u ):" "ncol=%d, prev_ncol=%d, nrow=%d\n", __func__, page, total_rows, ncol, prev_ncol, nrow ));


    for (p = 0; p < total_rows; p++)
    {
	if (PSCR(r, page).text[p])
	{
	    PSCR(r, page).text[p] = rxvt_realloc (
		PSCR(r, page).text[p], ncol * sizeof(text_t));
	    PSCR(r, page).rend[p] = rxvt_realloc (
		PSCR(r, page).rend[p], ncol * sizeof(rend_t));
	    MIN_IT(PSCR(r, page).tlen[p], (int16_t)ncol);
	    if (ncol > prev_ncol)
		rxvt_blank_line (
		    &(PSCR(r, page).text[p][prev_ncol]),
		    &(PSCR(r, page).rend[p][prev_ncol]),
		    ncol - prev_ncol, DEFAULT_RSTYLE);
	}
    }

    for (p = 0; p < nrow; p++)
    {
	PVTS(r, page)->drawn_text[p] = rxvt_realloc (
	    PVTS(r, page)->drawn_text[p], ncol * sizeof(text_t));
	PVTS(r, page)->drawn_rend[p] = rxvt_realloc (
	    PVTS(r, page)->drawn_rend[p], ncol * sizeof(rend_t));
#if NSCREENS
	if (PVTS(r, page)->swap.text[p])
	{
	    PVTS(r, page)->swap.text[p] = rxvt_realloc (
		PVTS(r, page)->swap.text[p], ncol * sizeof(text_t));
	    PVTS(r, page)->swap.rend[p] = rxvt_realloc (
		PVTS(r, page)->swap.rend[p], ncol * sizeof(rend_t));
	    MIN_IT(PVTS(r, page)->swap.tlen[p], (int16_t)ncol);
	    if (ncol > prev_ncol)
		rxvt_blank_line(
		    &(PVTS(r, page)->swap.text[p][prev_ncol]),
		    &(PVTS(r, page)->swap.rend[p][prev_ncol]),
		    ncol - prev_ncol, DEFAULT_RSTYLE);
	}
#endif
	if (ncol > prev_ncol)
	    rxvt_blank_line(
		&(PVTS(r, page)->drawn_text[p][prev_ncol]),
		&(PVTS(r, page)->drawn_rend[p][prev_ncol]),
		ncol - prev_ncol, DEFAULT_RSTYLE);
    }
    MIN_IT(PSCR(r, page).cur.col, (int16_t)ncol - 1);
#if NSCREENS
    MIN_IT(PVTS(r, page)->swap.cur.col, (int16_t)ncol - 1);
#endif


    /*
    ** Only reset tabstop if expanding columns, save realloc in
    ** shrinking columns
    */
    if (r->tabstop && ncol > prev_ncol)
    {
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "expand r->tabstop to %d\n", ncol));
	r->tabstop = rxvt_realloc(r->tabstop, ncol * sizeof(char));
	for (p = prev_ncol; p < ncol; p++)
	    r->tabstop[p] = (p % TABSTOP_SIZE == 0) ? 1 : 0;
    }
}