void
scr_erase_line(int mode)
{
    int             row, col, num;

    D_SCREEN((stderr, "scr_erase_line(%d) at screen row: %d", mode, screen.cur.row));
    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    if (Gr_Displayed())
	Gr_scroll(0);
    CHECK_SELECTION(1);

    if (screen.flags & Screen_WrapNext)
	screen.flags &= ~Screen_WrapNext;

    row = TermWin.saveLines + screen.cur.row;
    switch (mode) {
    case 0:			/* erase to end of line */
	col = screen.cur.col;
	num = TermWin.bcol - col;
	MIN_IT(screen.tlen[row], col);
	if (ROWCOL_IN_ROW_ON_OR_AFTER(selection.beg, screen.cur)
	    || ROWCOL_IN_ROW_ON_OR_AFTER(selection.end, screen.cur))
	    CLEAR_SELECTION;
	break;
    case 1:			/* erase to beginning of line */
	col = 0;
	num = screen.cur.col + 1;
	if (ROWCOL_IN_ROW_ON_OR_BEFORE(selection.beg, screen.cur)
	    || ROWCOL_IN_ROW_ON_OR_BEFORE(selection.end, screen.cur))
	    CLEAR_SELECTION;
	break;
    case 2:			/* erase whole line */
	col = 0;
	num = TermWin.bcol;
	screen.tlen[row] = 0;
	if (selection.beg.row <= screen.cur.row
	    || selection.end.row >= screen.cur.row)
	    CLEAR_SELECTION;
	break;
    default:
	return;
    }
    blank_line(&(screen.text[row][col]), &(screen.rend[row][col]), num,
	       rstyle & ~RS_Uline);
}
