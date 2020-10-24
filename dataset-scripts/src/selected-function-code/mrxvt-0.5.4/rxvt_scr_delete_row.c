void
rxvt_scr_delete_row (rxvt_t* r, int page)
{
    unsigned int    nrow, prev_nrow;
    unsigned int    p, q;
    register int    i;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_delete_row %d ()\n", page));
    nrow = r->TermWin.nrow;
    prev_nrow = PVTS(r, page)->prev_nrow;

    /* delete rows */
    i = min(PVTS(r, page)->nscrolled, prev_nrow - nrow);
    rxvt_scroll_text(r, page, 0, (int)prev_nrow - 1, i, 1);

    for (p = nrow; p < prev_nrow; p++)
    {
	q = p + SVLINES;
	if (PSCR(r, page).text[q])
	{
	    assert(PSCR(r, page).rend[q]);
	    rxvt_free(PSCR(r, page).text[q]);
	    PSCR(r, page).text[q] = NULL;
	    rxvt_free(PSCR(r, page).rend[q]);
	    PSCR(r, page).rend[q] = NULL;
	}
#if NSCREENS
	if (PVTS(r, page)->swap.text[p])
	{
	    assert(PVTS(r, page)->swap.rend[p]);
	    rxvt_free(PVTS(r, page)->swap.text[p]);
	    PVTS(r, page)->swap.text[p] = NULL;
	    rxvt_free(PVTS(r, page)->swap.rend[p]);
	    PVTS(r, page)->swap.rend[p] = NULL;
	}
#endif
	assert (PVTS(r, page)->drawn_text[p]);
	assert (PVTS(r, page)->drawn_rend[p]);
	rxvt_free(PVTS(r, page)->drawn_text[p]);
	PVTS(r, page)->drawn_text[p] = NULL;
	rxvt_free(PVTS(r, page)->drawn_rend[p]);
	PVTS(r, page)->drawn_rend[p] = NULL;
    }

    /* we have fewer rows so fix up cursor position */
    MIN_IT(PSCR(r, page).cur.row, (int32_t)nrow - 1);
#if NSCREENS
    MIN_IT(PVTS(r, page)->swap.cur.row, (int32_t)nrow - 1);
#endif

    rxvt_scr_reset_realloc (r, page);	/* realloc _last_ */
}