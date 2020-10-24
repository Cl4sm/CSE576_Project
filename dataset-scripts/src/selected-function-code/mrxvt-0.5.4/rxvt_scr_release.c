void
rxvt_scr_release(rxvt_t* r, int page)
{
    unsigned int    total_rows;
    int		    i;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_release %d ()\n", page));
    total_rows = r->TermWin.nrow + SVLINES;

    for (i = 0; i < total_rows; i++)
    {
	if (PSCR(r, page).text[i])
	{
	    /* then so is PSCR(r, page).rend[i] */
	    rxvt_free(PSCR(r, page).text[i]);
	    PSCR(r, page).text[i] = NULL;
	    assert(PSCR(r, page).rend[i]);
	    rxvt_free(PSCR(r, page).rend[i]);
	    PSCR(r, page).rend[i] = NULL;
	}
    }

    for (i = 0; i < r->TermWin.nrow; i++)
    {
	/* if (PVTS(r, page)->drawn_text[i]) */
	    rxvt_free(PVTS(r, page)->drawn_text[i]);
	PVTS(r, page)->drawn_text[i] = NULL;
	/* if (PVTS(r, page)->drawn_rend[i]) */
	    rxvt_free(PVTS(r, page)->drawn_rend[i]);
	PVTS(r, page)->drawn_rend[i] = NULL;
#if NSCREENS
	/* if (PVTS(r, page)->swap.text[i]) */
	    rxvt_free(PVTS(r, page)->swap.text[i]);
	PVTS(r, page)->swap.text[i] = NULL;
	/* if (PVTS(r, page)->swap.rend[i])) */
	    rxvt_free(PVTS(r, page)->swap.rend[i]);
	PVTS(r, page)->swap.rend[i] = NULL;
#endif
    }

    rxvt_free(PSCR(r, page).text); PSCR(r, page).text = NULL;
    rxvt_free(PSCR(r, page).tlen); PSCR(r, page).tlen = NULL;
    rxvt_free(PSCR(r, page).rend); PSCR(r, page).rend = NULL;
    rxvt_free(PVTS(r, page)->drawn_text);  PVTS(r, page)->drawn_text = NULL;
    rxvt_free(PVTS(r, page)->drawn_rend);  PVTS(r, page)->drawn_rend = NULL;
#if NSCREENS
    rxvt_free(PVTS(r, page)->swap.text);   PVTS(r, page)->swap.text = NULL;
    rxvt_free(PVTS(r, page)->swap.tlen);   PVTS(r, page)->swap.tlen = NULL;
    rxvt_free(PVTS(r, page)->swap.rend);   PVTS(r, page)->swap.rend = NULL;
#endif
    rxvt_free(PVTS(r, page)->buf_text);	PVTS(r, page)->buf_text = NULL;
    rxvt_free(PVTS(r, page)->buf_rend);	PVTS(r, page)->buf_rend = NULL;

    /* next rxvt_scr_reset will be the first time initialization */
    PVTS(r, page)->init_screen = 0;

    /* clear selection if necessary */
    if (page == r->selection.vt)
    {
	rxvt_process_selectionclear (r, page);
    }
}