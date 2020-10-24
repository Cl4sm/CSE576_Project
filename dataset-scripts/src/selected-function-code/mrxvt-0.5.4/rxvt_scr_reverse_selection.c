/* ------------------------------------------------------------------------- */
/* INTPROTO */
void
rxvt_scr_reverse_selection(rxvt_t* r, int page)
{
    int		 i, col, row, end_row;
    rend_t	 *srp;

    if (
	  SEL(r).op && SEL(r).vt == page &&
	  PVTS(r, page)->current_screen == SEL(r).screen
       )
    {
	end_row = SVLINES - VSTART;

	i = SEL(r).beg.row + SVLINES;
	row = SEL(r).end.row + SVLINES;

	if (i >= end_row)
	    col = SEL(r).beg.col;
	else
	{
	    col = 0;
	    i = end_row;
	}

	end_row += r->TermWin.nrow;
	for (; i < row && i < end_row; i++, col = 0)
	    for (srp = PSCR(r, page).rend[i]; col < r->TermWin.ncol; col++)
#ifndef OPTION_HC
		srp[col] ^= RS_RVid;
#else
		srp[col] ^= RS_Blink;
#endif
	if (i == row && i < end_row)
	    for (srp = PSCR(r, page).rend[i]; col < SEL(r).end.col; col++)
#ifndef OPTION_HC
		srp[col] ^= RS_RVid;
#else
		srp[col] ^= RS_Blink;
#endif