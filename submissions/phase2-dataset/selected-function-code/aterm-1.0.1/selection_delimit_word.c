selection_delimit_word(int dirn, row_col_t * mark, row_col_t * ret)
{
    int             col, row, dirnadd, tcol, trow, w1, w2;
    row_col_t       bound;
    text_t         *stp;
    rend_t         *srp;

    if (selection.clicks < 2)
	return;			/* Go away: we only handle double clicks */

    if (dirn == UP) {
	bound.row = TermWin.saveLines - TermWin.nscrolled - 1;
	bound.col = 0;
	dirnadd = -1;
    } else {
	bound.row = TermWin.saveLines + TermWin.nrow;
	bound.col = TermWin.bcol - 1;
	dirnadd = 1;
    }
    row = mark->row + TermWin.saveLines;
    col = mark->col;
    MAX_IT(col, 0);
/* find the edge of a word */
    stp = &(screen.text[row][col]);
    w1 = DELIMIT_TEXT(*stp);
#ifdef OLD_WORD_SELECTION
    if (w1 == 1) {
	stp += dirnadd;
	if (DELIMIT_TEXT(*stp) == 1)
	    goto Old_Word_Selection_You_Die;
	col += dirnadd;
	srp += dirnadd;
    }
    w1 = 0;
#endif
    srp = (&screen.rend[row][col]);
    w2 = DELIMIT_REND(*srp);

    for (;;) {
	for (; col != bound.col; col += dirnadd) {
	    stp += dirnadd;
	    if (DELIMIT_TEXT(*stp) != w1)
		break;
	    srp += dirnadd;
	    if (DELIMIT_REND(*srp) != w2)
		break;
	}
	if ((col == bound.col) && (row != bound.row)) {
	    if (screen.tlen[(row - (dirn == UP))] == -1) {
		trow = row + dirnadd;
		tcol = (dirn == UP) ? (TermWin.bcol - 1) : 0;
		if (screen.text[trow] == NULL)
		    break;
		stp = &(screen.text[trow][tcol]);
		srp = &(screen.rend[trow][tcol]);
		if (DELIMIT_TEXT(*stp) != w1 || DELIMIT_REND(*srp) != w2)
		    break;
		row = trow;
		col = tcol;
		continue;
	    }
	}
	break;
    }
#ifdef OLD_WORD_SELECTION
Old_Word_Selection_You_Die:
#endif
    D_SELECT((stderr, "selection_delimit_word(%s,...) @ (r:%3d, c:%3d) has boundary (r:%3d, c:%3d)", (dirn == UP ? "up	" : "down"), mark->row, mark->col, row - TermWin.saveLines, col));

    if (dirn == DN)
	col++;			/* put us on one past the end */

/* Poke the values back in */
    ret->row = row - TermWin.saveLines;
    ret->col = col;
}
