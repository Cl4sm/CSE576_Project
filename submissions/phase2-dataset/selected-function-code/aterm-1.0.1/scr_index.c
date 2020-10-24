void
scr_index(int direction)
{
    int             dirn;

    dirn = ((direction == UP) ? 1 : -1);
    D_SCREEN((stderr, "scr_index(%d)", dirn));

    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    if (Gr_Displayed())
	Gr_scroll(0);

    if (screen.flags & Screen_WrapNext) {
	screen.flags &= ~Screen_WrapNext;
    }
    if ((screen.cur.row == screen.bscroll && direction == UP)
	|| (screen.cur.row == screen.tscroll && direction == DN)) {
	scroll_text(screen.tscroll, screen.bscroll, dirn, 0);
	if (direction == UP)
	    dirn = screen.bscroll + TermWin.saveLines;
	else
	    dirn = screen.tscroll + TermWin.saveLines;
	if (screen.text[dirn] == NULL)	/* then so is screen.rend[dirn] */
	    make_screen_mem(screen.text, screen.rend, dirn, rstyle);
	else
	    blank_line(screen.text[dirn], screen.rend[dirn], TermWin.bcol,
		       rstyle);
	screen.tlen[dirn] = 0;
    } else
	screen.cur.row += dirn;
    MAX_IT(screen.cur.row, 0);
    MIN_IT(screen.cur.row, TermWin.nrow - 1);
    CHECK_SELECTION(0);
}
