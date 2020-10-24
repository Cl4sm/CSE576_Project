scr_insdel_chars(int count, int insdel)
{
    int             col, row;
    rend_t	    tr;

    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    if (Gr_Displayed())
	Gr_scroll(0);

    if (count <= 0)
	return;
    CHECK_SELECTION(1);
    MIN_IT(count, (TermWin.bcol - screen.cur.col));

    row = screen.cur.row + TermWin.saveLines;
    screen.flags &= ~Screen_WrapNext;

    switch (insdel) {
    case INSERT:
	for (col = TermWin.bcol - 1; (col - count) >= screen.cur.col; col--) {
	    screen.text[row][col] = screen.text[row][col - count];
	    screen.rend[row][col] = screen.rend[row][col - count];
	}
	if (screen.tlen[row] != -1) {
	    screen.tlen[row] += count;
	    MIN_IT(screen.tlen[row], TermWin.bcol);
	}
	if (selection.op && current_screen == selection.screen
	    && ROWCOL_IN_ROW_ON_OR_AFTER(selection.beg, screen.cur)) {
	    if (selection.end.row != screen.cur.row
		|| (selection.end.col + count >= TermWin.bcol))
		CLEAR_SELECTION;
	    else {		/* shift selection */
		selection.beg.col += count;
		selection.mark.col += count;	/* XXX: yes? */
		selection.end.col += count;
	    }
	}
	blank_line(&(screen.text[row][screen.cur.col]),
		   &(screen.rend[row][screen.cur.col]),
		   count, rstyle);
	break;
    case ERASE:
	screen.cur.col += count;	/* don't worry if > TermWin.bcol */
	CHECK_SELECTION(1);
	screen.cur.col -= count;
	blank_line(&(screen.text[row][screen.cur.col]),
		   &(screen.rend[row][screen.cur.col]),
		   count, rstyle);
	break;
    case DELETE:
	tr = screen.rend[row][TermWin.bcol - 1]
	     & (RS_fgMask | RS_bgMask | RS_baseattrMask);
	for (col = screen.cur.col; (col + count) < TermWin.bcol; col++) {
	    screen.text[row][col] = screen.text[row][col + count];
	    screen.rend[row][col] = screen.rend[row][col + count];
	}
	blank_line(&(screen.text[row][TermWin.bcol - count]),
		   &(screen.rend[row][TermWin.bcol - count]),
		   count, tr);
	if (screen.tlen[row] == -1)	/* break line continuation */
	    screen.tlen[row] = TermWin.bcol;
	screen.tlen[row] -= count;
	MAX_IT(screen.tlen[row], 0);
	if (selection.op && current_screen == selection.screen
	    && ROWCOL_IN_ROW_ON_OR_AFTER(selection.beg, screen.cur)) {
	    if (selection.end.row != screen.cur.row
		|| (screen.cur.col >= selection.beg.col - count)
		|| selection.end.col >= TermWin.bcol)
		CLEAR_SELECTION;
	    else {
		/* shift selection */
		selection.beg.col -= count;
		selection.mark.col -= count;	/* XXX: yes? */
		selection.end.col -= count;
	    }
	}
	break;
    }
#ifdef MULTICHAR_SET
    if ((screen.rend[row][0] & RS_multiMask) == RS_multi2) {
	screen.rend[row][0] &= ~RS_multiMask;
	screen.text[row][0] = ' ';
    }
    if ((screen.rend[row][TermWin.bcol - 1] & RS_multiMask) == RS_multi1) {
	screen.rend[row][TermWin.bcol - 1] &= ~RS_multiMask;
	screen.text[row][TermWin.bcol - 1] = ' ';
    }
#endif
}
