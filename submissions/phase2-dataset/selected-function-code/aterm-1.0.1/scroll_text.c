scroll_text(int row1, int row2, int count, int spec)
{
    int             i, j;

    D_SCREEN((stderr, "scroll_text(%d,%d,%d,%d): %s", row1, row2, count, spec, (current_screen == PRIMARY) ? "Primary" : "Secondary"));

    if (count == 0 || (row1 > row2))
	return 0;

    if ((count > 0) && (row1 == 0) && (current_screen == PRIMARY)) {
	if (TermWin.saveLines - TermWin.nscrolled < count)
		TermWin.nscrolled += TermWin.saveLines - TermWin.nscrolled;
	else
		TermWin.nscrolled += count;
	MIN_IT(TermWin.nscrolled, TermWin.saveLines);
    } else if (!spec)
	row1 += TermWin.saveLines;
    row2 += TermWin.saveLines;

    if (selection.op && current_screen == selection.screen) {
	i = selection.beg.row + TermWin.saveLines;
	j = selection.end.row + TermWin.saveLines;
	if ((i < row1 && j > row1)
	    || (i < row2 && j > row2)
	    || (i - count < row1 && i >= row1)
	    || (i - count > row2 && i <= row2)
	    || (j - count < row1 && j >= row1)
	    || (j - count > row2 && j <= row2)) {
	    CLEAR_ALL_SELECTION;
	    selection.op = SELECTION_CLEAR;	/* XXX: too aggressive? */
	} else if (j >= row1 && j <= row2) {
	/* move selected region too */
	    selection.beg.row -= count;
	    selection.end.row -= count;
	    selection.mark.row -= count;
	}
    }
    CHECK_SELECTION(0);		/* _after_ TermWin.nscrolled update */

    if (count > 0) {
/* A: scroll up */

	MIN_IT(count, row2 - row1 + 1);
/* A1: Copy lines that will get clobbered by the rotation */
	for (i = 0, j = row1; i < count; i++, j++) {
	    buf_text[i] = screen.text[j];
	    buf_tlen[i] = screen.tlen[j];
	    buf_rend[i] = screen.rend[j];
	}
/* A2: Rotate lines */
	for (j = row1; (j + count) <= row2; j++) {
	    screen.text[j] = screen.text[j + count];
	    screen.tlen[j] = screen.tlen[j + count];
	    screen.rend[j] = screen.rend[j + count];
	}
/* A3: Resurrect lines */
	for (i = 0; i < count; i++, j++) {
	    screen.text[j] = buf_text[i];
	    screen.tlen[j] = buf_tlen[i];
	    screen.rend[j] = buf_rend[i];
	}
    } else if (count < 0) {
/* B: scroll down */

	count = min(-count, row2 - row1 + 1);
/* B1: Copy lines that will get clobbered by the rotation */
	for (i = 0, j = row2; i < count; i++, j--) {
	    buf_text[i] = screen.text[j];
	    buf_tlen[i] = screen.tlen[j];
	    buf_rend[i] = screen.rend[j];
	}
/* B2: Rotate lines */
	for (j = row2; (j - count) >= row1; j--) {
	    screen.text[j] = screen.text[j - count];
	    screen.tlen[j] = screen.tlen[j - count];
	    screen.rend[j] = screen.rend[j - count];
	}
/* B3: Resurrect lines */
	for (i = 0, j = row1; i < count; i++, j++) {
	    screen.text[j] = buf_text[i];
	    screen.tlen[j] = buf_tlen[i];
	    screen.rend[j] = buf_rend[i];
	}
	count = -count;
    }
    if (Gr_Displayed())
	Gr_scroll(count);
    return count;
}
