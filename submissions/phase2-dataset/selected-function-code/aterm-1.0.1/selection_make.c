void
selection_make(Time tm, unsigned int key_state)
{
    int             i, col, end_col, row, end_row;
    unsigned char  *new_selection_text;
    char           *str;
    text_t         *t;

    D_SELECT((stderr, "selection_make(): selection.op=%d, selection.clicks=%d", selection.op, selection.clicks));
    switch (selection.op) {
    case SELECTION_CONT:
	break;
    case SELECTION_INIT:
	CLEAR_SELECTION;
    /* FALLTHROUGH */
    case SELECTION_BEGIN:
	selection.op = SELECTION_DONE;
    /* FALLTHROUGH */
    default:
	return;
    }
    selection.op = SELECTION_DONE;

    if (selection.clicks == 4)
	return;			/* nothing selected, go away */

    i = (selection.end.row - selection.beg.row + 1) * (TermWin.bcol + 1) + 1;
    str = MALLOC(i * sizeof(char));
    new_selection_text = (unsigned char *) str;

    col = max((int)selection.beg.col, 0);
    row = selection.beg.row + TermWin.saveLines;
    end_row = selection.end.row + TermWin.saveLines;
/*
 * A: rows before end row
 */
    for (; row < end_row; row++) {
	t = &(screen.text[row][col]);
	if ((end_col = screen.tlen[row]) == -1)
	    end_col = TermWin.bcol;
	for (; col < end_col; col++)
	    *str++ = *t++;
	col = 0;
	if (screen.tlen[row] != -1)
	    *str++ = '\n';
    }
/*
 * B: end row
 */
    t = &(screen.text[row][col]);
    end_col = screen.tlen[row];
    if (end_col == -1 || selection.end.col <= end_col)
	end_col = selection.end.col;
    MIN_IT(end_col, TermWin.bcol);	/* CHANGE */
    for (; col < end_col; col++)
	*str++ = *t++;
    if (end_col != selection.end.col)
	*str++ = '\n';
    *str = '\0';
    if ((i = strlen((char *) new_selection_text)) == 0) {
	FREE(new_selection_text);
	return;
    }
    selection.len = i;
    if (selection.text)
	FREE(selection.text);
    selection.text = new_selection_text;

    // selecting with ALT will put the text to clipboard
    if (key_state & Mod1Mask) {
        XSetSelectionOwner(Xdisplay, aterm_XA_CLIPBOARD, TermWin.vt, tm);
    } else {
        XSetSelectionOwner(Xdisplay, XA_PRIMARY, TermWin.vt, tm);
    }
	
    if (XGetSelectionOwner(Xdisplay, XA_PRIMARY) != TermWin.vt)
	print_error("can't get primary selection");
    XChangeProperty(Xdisplay, Xroot, XA_CUT_BUFFER0, XA_STRING, 8,
		    PropModeReplace, selection.text, selection.len);
    D_SELECT((stderr, "selection_make(): selection.len=%d", selection.len));
}
