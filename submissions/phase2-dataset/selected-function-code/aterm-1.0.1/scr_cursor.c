scr_cursor(int mode)
{
    D_SCREEN((stderr, "scr_cursor(%c)", mode));

    switch (mode) {
    case SAVE:
	save.cur.row = screen.cur.row;
	save.cur.col = screen.cur.col;
	save.rstyle = rstyle;
	save.charset = screen.charset;
	save.charset_char = charsets[screen.charset];
	break;
    case RESTORE:
	screen.cur.row = save.cur.row;
	screen.cur.col = save.cur.col;
	rstyle = save.rstyle;
	screen.charset = save.charset;
	charsets[screen.charset] = save.charset_char;
	set_font_style();
	break;
    }
/* boundary check in case screen size changed between SAVE and RESTORE */
    MIN_IT(screen.cur.row, TermWin.nrow - 1);
    MIN_IT(screen.cur.col, TermWin.bcol - 1);
#ifdef DEBUG_STRICT
    assert(screen.cur.row >= 0);
    assert(screen.cur.col >= 0);
#else				/* drive with your eyes closed */
    MAX_IT(screen.cur.row, 0);
    MAX_IT(screen.cur.col, 0);
#endif
}
