void
rxvt_scr_E(rxvt_t* r, int page)
{
    int		 i, j, k;
    rend_t	 *r1, fs;

    PVTS(r, page)->want_refresh = 1;
    r->h->num_scr_allow = 0;
    ZERO_SCROLLBACK(r, page);
    RESET_CHSTAT(r, page);
    rxvt_selection_check(r, page, 3);

    fs = PVTS(r, page)->rstyle;
    for (k = SVLINES, i = r->TermWin.nrow; i--; k++)
    {
	/* make the `E's selectable */
	PSCR(r, page).tlen[k] = r->TermWin.ncol;
	MEMSET(PSCR(r, page).text[k], 'E', r->TermWin.ncol);
	for (r1 = PSCR(r, page).rend[k], j = r->TermWin.ncol; j--; )
	    *r1++ = fs;
    }
}