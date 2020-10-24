void
scr_gotorc(int row, int col, int relative)
{
    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    if (Gr_Displayed())
	Gr_scroll(0);

    D_SCREEN((stderr, "scr_gotorc(r:%d,c:%d,%d): from (r:%d,c:%d)", row, col, relative, screen.cur.row, screen.cur.col));

    screen.cur.col = ((relative & C_RELATIVE) ? (screen.cur.col + col) : col);
    MAX_IT(screen.cur.col, 0);
    MIN_IT(screen.cur.col, TermWin.bcol - 1);

    if (screen.flags & Screen_WrapNext) {
	screen.flags &= ~Screen_WrapNext;
    }
    if (relative & R_RELATIVE) {
	if (row > 0) {
	    if (screen.cur.row <= screen.bscroll
		&& (screen.cur.row + row) > screen.bscroll)
		screen.cur.row = screen.bscroll;
	    else
		screen.cur.row += row;
	} else if (row < 0) {
	    if (screen.cur.row >= screen.tscroll
		&& (screen.cur.row + row) < screen.tscroll)
		screen.cur.row = screen.tscroll;
	    else
		screen.cur.row += row;
	}
    } else {
	if (screen.flags & Screen_Relative) {	/* relative origin mode */
	    screen.cur.row = row + screen.tscroll;
	    MIN_IT(screen.cur.row, screen.bscroll);
	} else
	    screen.cur.row = row;
    }
    MAX_IT(screen.cur.row, 0);
    MIN_IT(screen.cur.row, TermWin.nrow - 1);
}
