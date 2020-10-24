scr_insdel_lines(int count, int insdel)
{
    int             end;

    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    if (Gr_Displayed())
	Gr_scroll(0);
    CHECK_SELECTION(1);

    if (screen.cur.row > screen.bscroll)
	return;

    end = screen.bscroll - screen.cur.row + 1;
    if (count > end) {
	if (insdel == DELETE)
	    return;
	else if (insdel == INSERT)
	    count = end;
    }
    if (screen.flags & Screen_WrapNext) {
	screen.flags &= ~Screen_WrapNext;
    }
    scroll_text(screen.cur.row, screen.bscroll, insdel * count, 0);

/* fill the inserted or new lines with rstyle. TODO: correct for delete? */
    if (insdel == DELETE) {
	end = screen.bscroll + TermWin.saveLines;
    } else if (insdel == INSERT) {
	end = screen.cur.row + count - 1 + TermWin.saveLines;
    }
    for (; count--; end--) {
	if (screen.text[end] == NULL)	/* then so is screen.rend[end] */
	    make_screen_mem(screen.text, screen.rend, end, rstyle);
	else
	    blank_line(screen.text[end], screen.rend[end], TermWin.bcol,
		       rstyle);
	screen.tlen[end] = 0;
    }
}
