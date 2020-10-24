void
rxvt_scr_reset(rxvt_t* r, int page)
{
    unsigned int    ncol, nrow, prev_ncol, prev_nrow,
		    total_rows, prev_total_rows;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_reset %d ()\n", page));

    VSTART = 0;
    RESET_CHSTAT(r, page);
    PVTS(r, page)->num_scr = 0;	/* number of lines scrolled */

    prev_ncol = PVTS(r, page)->prev_ncol;
    prev_nrow = PVTS(r, page)->prev_nrow;
    if (r->TermWin.ncol == 0)
	r->TermWin.ncol = 80;
    if (r->TermWin.nrow == 0)
	r->TermWin.nrow = 24;
    ncol = r->TermWin.ncol;
    nrow = r->TermWin.nrow;
    if (PVTS(r, page)->init_screen &&
	ncol == prev_ncol && nrow == prev_nrow)
	return;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_reset %d () refresh screen\n", page));
    PVTS(r, page)->want_refresh = 1;

    total_rows = nrow + SVLINES;
    prev_total_rows = prev_nrow + SVLINES;

    PSCR(r, page).tscroll = 0;
    PSCR(r, page).bscroll = nrow - 1;

    if (PVTS(r, page)->init_screen == 0)
    {
	/* Initialize the screen structures */
	rxvt_scr_alloc (r, page);
    }
    else
    {
	/* B1: resize rows */
	if (nrow < prev_nrow)
	{
	    rxvt_scr_delete_row (r, page);
	}
	else if (nrow > prev_nrow)
	{
	    rxvt_scr_add_row (r, page, total_rows, prev_total_rows);
	}
	/* B2: resize columns */
	if (ncol != prev_ncol)
	{
	    rxvt_scr_adjust_col (r, page, total_rows);
	}
    }

    PVTS(r, page)->prev_nrow = nrow;
    PVTS(r, page)->prev_ncol = ncol;

    rxvt_tt_winsize(PVTS(r, page)->cmd_fd, r->TermWin.ncol, r->TermWin.nrow, PVTS(r, page)->cmd_pid);
}