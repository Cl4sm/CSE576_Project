void
selection_check(int check_more)
{
    row_col_t       pos;

    if ((selection.beg.row < -TermWin.nscrolled)
	|| (selection.beg.row >= TermWin.nrow)
	|| (selection.mark.row < -TermWin.nscrolled)
	|| (selection.mark.row >= TermWin.nrow)
	|| (selection.end.row < -TermWin.nscrolled)
	|| (selection.end.row >= TermWin.nrow))
	CLEAR_ALL_SELECTION;

    if (check_more == 1 && current_screen == selection.screen) {
    /* check for cursor position */
	pos.row = screen.cur.row;
	pos.col = screen.cur.col;
	if (!ROWCOL_IS_BEFORE(pos, selection.beg)
	    && ROWCOL_IS_BEFORE(pos, selection.end))
	    CLEAR_SELECTION;
    } else if (check_more == 2) {
	pos.row = 0;
	pos.col = 0;
	if (ROWCOL_IS_BEFORE(selection.beg, pos)
	    && ROWCOL_IS_AFTER(selection.end, pos))
	    CLEAR_SELECTION;
    } else if (check_more == 3) {
	pos.row = 0;
	pos.col = 0;
	if (ROWCOL_IS_AFTER(selection.end, pos))
	    CLEAR_SELECTION;
    }
}
