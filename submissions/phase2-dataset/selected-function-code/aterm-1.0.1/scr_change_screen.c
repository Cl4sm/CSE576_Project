scr_change_screen(int scrn)
{
    int             i, offset, tmp;
    text_t         *t0;
    rend_t         *r0;
    R_int16_t	    l0;

    D_SCREEN((stderr, "scr_change_screen(%d)", scrn));

    TermWin.view_start = 0;
    RESET_CHSTAT;

    if (current_screen == scrn)
	return current_screen;

    CHECK_SELECTION(2);		/* check for boundary cross */

    SWAP_IT(current_screen, scrn, tmp);
#if NSCREENS
    offset = TermWin.saveLines;
    for (i = TermWin.nrow; i--;) {
	SWAP_IT(screen.text[i + offset], swap.text[i], t0);
	SWAP_IT(screen.tlen[i + offset], swap.tlen[i], l0);
	SWAP_IT(screen.rend[i + offset], swap.rend[i], r0);
    }
    SWAP_IT(screen.cur.row, swap.cur.row, l0);
    SWAP_IT(screen.cur.col, swap.cur.col, l0);
# ifdef DEBUG_STRICT
    assert(screen.cur.row >= 0);
    assert(screen.cur.col >= 0);
    assert(screen.cur.row < TermWin.nrow);
    assert(screen.cur.col < TermWin.bcol);
# else				/* drive with your eyes closed */
    MAX_IT(screen.cur.row, 0);
    MAX_IT(screen.cur.col, 0);
    MIN_IT(screen.cur.row, TermWin.nrow - 1);
    MIN_IT(screen.cur.col, TermWin.bcol - 1);
# endif
    SWAP_IT(screen.charset, swap.charset, l0);
    SWAP_IT(screen.flags, swap.flags, tmp);
    screen.flags |= Screen_VisibleCursor;
    swap.flags |= Screen_VisibleCursor;

    if (Gr_Displayed()) {
	Gr_scroll(0);
	Gr_ChangeScreen();
    }
#else
# ifndef DONT_SCROLL_ME
    if (Gr_Displayed())
	Gr_ClearScreen();
    if (current_screen == PRIMARY) {
	if (!Gr_Displayed())
	    scroll_text(0, (TermWin.nrow - 1), TermWin.nrow, 0);
	for (i = TermWin.saveLines; i < TermWin.nrow + TermWin.saveLines; i++)
	    if (screen.text[i] == NULL) {
		make_screen_mem(screen.text, screen.rend, i, DEFAULT_RSTYLE);
		screen.tlen[i] = 0;
	    }
    }
# endif
#endif
    return scrn;
}
