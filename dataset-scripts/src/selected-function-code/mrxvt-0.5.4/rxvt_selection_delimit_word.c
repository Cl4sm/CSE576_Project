#endif

/* INTPROTO */
void
rxvt_selection_delimit_word(rxvt_t* r, int page, enum page_dirn dirn, const row_col_t *mark, row_col_t *ret)
{
    int		 col, row, dirnadd, tcol, trow, w1, w2;
    row_col_t	   bound;
    text_t	 *stp;
    rend_t	 *srp;


    r->selection.vt = page; /* update selection vt */

    if (dirn == UP)
    {
	bound.row = SVLINES - PVTS(r, page)->nscrolled - 1;
	bound.col = 0;
	dirnadd = -1;
    }
    else
    {
	bound.row = SVLINES + r->TermWin.nrow;
	bound.col = r->TermWin.ncol - 1;
	dirnadd = 1;
    }
    row = mark->row + SVLINES;
    col = mark->col;
    MAX_IT(col, 0);
/* find the edge of a word */
    stp = &(PSCR(r, page).text[row][col]);
    w1 = DELIMIT_TEXT(*stp);

    if (r->selection_style != NEW_SELECT)
    {
	if (w1 == 1)
	{
	    stp += dirnadd;
	    if (DELIMIT_TEXT(*stp) == 1)
		goto Old_Word_Selection_You_Die;
	    col += dirnadd;
	}
	w1 = 0;
    }
    srp = (&PSCR(r, page).rend[row][col]);
    w2 = DELIMIT_REND(*srp);

    for (;;)
    {
	for (; col != bound.col; col += dirnadd)
	{
	    stp += dirnadd;
	    if (DELIMIT_TEXT(*stp) != w1)
		break;
	    srp += dirnadd;
	    if (DELIMIT_REND(*srp) != w2)
		break;
	}
	if ((col == bound.col) && (row != bound.row))
	{
	    if (PSCR(r, page).tlen[(row - (dirn == UP ? 1 : 0))] == -1)
	    {
		trow = row + dirnadd;
		tcol = dirn == UP ? r->TermWin.ncol - 1 : 0;
		if (PSCR(r, page).text[trow] == NULL)
		    break;
		stp = &(PSCR(r, page).text[trow][tcol]);
		srp = &(PSCR(r, page).rend[trow][tcol]);
		if (DELIMIT_TEXT(*stp) != w1 ||
		    DELIMIT_REND(*srp) != w2)
		    break;
		row = trow;
		col = tcol;
		continue;
	    }
	}
	break;
    }

Old_Word_Selection_You_Die:
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_delimit_word %d (%s,...) @ (r:%3d, c:%3d) has boundary (r:%3d, c:%3d)\n", page, (dirn == UP ? "up    " : "down"), mark->row, mark->col, row - SVLINES, col));

    if (dirn == DN)
	col++;		/* put us on one past the end */

/* Poke the values back in */