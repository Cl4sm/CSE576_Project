scr_reverse_selection(void)
{
    int		i, col, row, end_row;
    rend_t     *srp;

    end_row = TermWin.saveLines - TermWin.view_start;
    if (selection.op && current_screen == selection.screen) {
	i = selection.beg.row + TermWin.saveLines;
	row = selection.end.row + TermWin.saveLines;
	if (i >= end_row)
	    col = selection.beg.col;
	else {
	    col = 0;
	    i = end_row;
	}
	end_row += TermWin.nrow;
	for (; i < row && i < end_row; i++, col = 0)
	    for (srp = screen.rend[i]; col < TermWin.bcol; col++)
		srp[col] ^= RS_RVid;
	if (i == row && i < end_row)
	    for (srp = screen.rend[i]; col < selection.end.col; col++)
		srp[col] ^= RS_RVid;
    }
}
