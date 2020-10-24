void
selection_extend_colrow(int col, int row, int button3, int buttonpress, int clickchange)
{
    int             end_col;
    row_col_t       pos;
    enum {
	LEFT, RIGHT
    } closeto =     RIGHT;
#ifdef MULTICHAR_SET
    int             c, r;
#endif

    D_SELECT((stderr, "selection_extend_colrow(c:%d, r:%d, %d, %d) clicks:%d", col, row, button3, buttonpress, selection.clicks));
    D_SELECT((stderr, "selection_extend_colrow() ENT  b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)", selection.beg.row, selection.beg.col, selection.mark.row, selection.mark.col, selection.end.row, selection.end.col));

    switch (selection.op) {
    case SELECTION_INIT:
	CLEAR_SELECTION;
	selection.op = SELECTION_BEGIN;
    /* FALLTHROUGH */
    case SELECTION_BEGIN:
	if (row != selection.mark.row || col != selection.mark.col
	    || (!button3 && buttonpress))
	    selection.op = SELECTION_CONT;
	break;
    case SELECTION_DONE:
	selection.op = SELECTION_CONT;
    /* FALLTHROUGH */
    case SELECTION_CONT:
	break;
    case SELECTION_CLEAR:
	selection_start_colrow(col, row);
    /* FALLTHROUGH */
    default:
	return;
    }

    pos.col = col;
    pos.row = row;

    pos.row -= TermWin.view_start;	/* adjust for scroll */

#ifdef OLD_SELECTION
/*
 * This mimics some of the selection behaviour of version 2.20 and before.
 * There are no ``selection modes'', button3 is always character extension.
 * Note: button3 drag is always available, c.f. v2.20
 * Selection always terminates (left or right as appropriate) at the mark.
 */
    {
	static int      hate_those_clicks = 0;	/* a.k.a. keep mark position */

	if (selection.clicks == 1 || button3) {
	    if (hate_those_clicks) {
		selection.mark.row = selection.beg.row;
		selection.mark.col = selection.beg.col;
		hate_those_clicks = 0;
	    }
	    if (ROWCOL_IS_BEFORE(pos, selection.beg)) {
		selection.end.row = selection.beg.row;
		selection.end.col = selection.beg.col + 1;
		selection.beg.row = pos.row;
		selection.beg.col = pos.col;
	    } else {
		selection.beg.row = selection.mark.row;
		selection.beg.col = selection.mark.col;
		selection.end.row = pos.row;
		selection.end.col = pos.col + 1;
	    }
	} else if (selection.clicks == 2) {
	    selection_delimit_word(UP, &(selection.mark), &(selection.beg));
	    selection_delimit_word(DN, &(selection.mark), &(selection.end));
	    hate_those_clicks = 1;
	} else if (selection.clicks == 3) {
	    selection.beg.row = selection.end.row = selection.mark.row;
	    selection.beg.col = 0;
	    selection.end.col = TermWin.bcol;
	    hate_those_clicks = 1;
	}
    }
#else				/* ! OLD_SELECTION */
/*
 * This is mainly xterm style selection with a couple of differences, mainly
 * in the way button3 drag extension works.
 * We're either doing: button1 drag; button3 press; or button3 drag
 *  a) button1 drag : select around a midpoint/word/line - that point/word/line
 *     is always at the left/right edge of the selection.
 *  b) button3 press: extend/contract character/word/line at whichever edge of
 *     the selection we are closest to.
 *  c) button3 drag : extend/contract character/word/line - we select around
 *     a point/word/line which is either the start or end of the selection
 *     and it was decided by whichever point/word/line was `fixed' at the
 *     time of the most recent button3 press
 */
    if (button3 && buttonpress) {	/* button3 press */
    /*
     * first determine which edge of the selection we are closest to
     */
	if (ROWCOL_IS_BEFORE(pos, selection.beg)
	    || (!ROWCOL_IS_AFTER(pos, selection.end)
		&& (((pos.col - selection.beg.col)
		     + ((pos.row - selection.beg.row) * TermWin.bcol))
		    < ((selection.end.col - pos.col)
		       + ((selection.end.row - pos.row) * TermWin.bcol)))))
	    closeto = LEFT;
	if (closeto == LEFT) {
	    selection.beg.row = pos.row;
	    selection.beg.col = pos.col;
	    selection.mark.row = selection.end.row;
	    selection.mark.col = selection.end.col - (selection.clicks == 2);
	} else {
	    selection.end.row = pos.row;
	    selection.end.col = pos.col;
	    selection.mark.row = selection.beg.row;
	    selection.mark.col = selection.beg.col;
	}
    } else {			/* button1 drag or button3 drag */
	if (ROWCOL_IS_AFTER(selection.mark, pos)) {
	    if ((selection.mark.row == selection.end.row)
		&& (selection.mark.col == selection.end.col)
		&& clickchange && selection.clicks == 2)
		selection.mark.col--;
	    selection.beg.row = pos.row;
	    selection.beg.col = pos.col;
	    selection.end.row = selection.mark.row;
	    selection.end.col = selection.mark.col + (selection.clicks == 2);
	} else {
	    selection.beg.row = selection.mark.row;
	    selection.beg.col = selection.mark.col;
	    selection.end.row = pos.row;
	    selection.end.col = pos.col;
	}
    }

    if (selection.clicks == 1) {
	end_col = screen.tlen[selection.beg.row + TermWin.saveLines];
	if (end_col != -1 && selection.beg.col > end_col) {
#if 1
	    selection.beg.col = TermWin.bcol;
#else
	    if (selection.beg.row != selection.end.row)
		selection.beg.col = TermWin.bcol;
	    else
		selection.beg.col = selection.mark.col;
#endif
	}
	end_col = screen.tlen[selection.end.row + TermWin.saveLines];
	if (end_col != -1 && selection.end.col > end_col)
	    selection.end.col = TermWin.bcol;

# ifdef MULTICHAR_SET
	if (selection.beg.col > 0) {
	    r = selection.beg.row + TermWin.saveLines;
	    c = selection.beg.col;
	    if (((screen.rend[r][c] & RS_multiMask) == RS_multi2)
		&& ((screen.rend[r][c - 1] & RS_multiMask) == RS_multi1))
		selection.beg.col--;
	}
	if (selection.end.col < TermWin.bcol) {
	    r = selection.end.row + TermWin.saveLines;
	    c = selection.end.col;
	    if (((screen.rend[r][c - 1] & RS_multiMask) == RS_multi1)
		&& ((screen.rend[r][c] & RS_multiMask) == RS_multi2))
		selection.end.col++;
	}
# endif				/* MULTICHAR_SET */
    } else if (selection.clicks == 2) {
	if (ROWCOL_IS_AFTER(selection.end, selection.beg))
	    selection.end.col--;
	selection_delimit_word(UP, &(selection.beg), &(selection.beg));
	selection_delimit_word(DN, &(selection.end), &(selection.end));
    } else if (selection.clicks == 3) {
	if (ROWCOL_IS_AFTER(selection.mark, selection.beg))
	    selection.mark.col++;
	if (Options & Opt_cutToBeginningOfLine) {
		selection_delimit_word(UP, &(selection.beg), &(selection.beg));
	} else {
		selection.beg.col = 0;
	}
	selection.end.col = TermWin.bcol;
    }
    if (button3 && buttonpress) {	/* mark may need to be changed */
	if (closeto == LEFT) {
	    selection.mark.row = selection.end.row;
	    selection.mark.col = selection.end.col - (selection.clicks == 2);
	} else {
	    selection.mark.row = selection.beg.row;
	    selection.mark.col = selection.beg.col;
	}
    }
#endif				/* ! OLD_SELECTION */
    D_SELECT((stderr, "selection_extend_colrow() EXIT b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)", selection.beg.row, selection.beg.col, selection.mark.row, selection.mark.col, selection.end.row, selection.end.col));
}
