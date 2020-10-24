 * Extend the selection to the specified col/row
 */
/* INTPROTO */
void
rxvt_selection_extend_colrow(rxvt_t* r, int page, int32_t col, int32_t row, int button3, int buttonpress, int clickchange)
{
    unsigned int    ncol = r->TermWin.ncol;
    row_col_t	    pos;
#ifndef NO_NEW_SELECTION
    int		 end_col;
    enum {
	LEFT, RIGHT
    } closeto = RIGHT;
#endif


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_extend_colrow %d (c:%d, r:%d, %d, %d) clicks:%d, op:%d\n", page, col, row, button3, buttonpress, SEL(r).clicks, SEL(r).op));
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_extend_colrow %d () ENT  b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)\n", page, SEL(r).beg.row, SEL(r).beg.col, SEL(r).mark.row, SEL(r).mark.col, SEL(r).end.row, SEL(r).end.col));

    PVTS(r, page)->want_refresh = 1;
    switch (SEL(r).op)
    {
	case SELECTION_INIT:
	    CLEAR_SELECTION(r);
	    SEL(r).op = SELECTION_BEGIN;
	    /* FALLTHROUGH */
	case SELECTION_BEGIN:
	    if (row != SEL(r).mark.row ||
		col != SEL(r).mark.col ||
		(!button3 && buttonpress))
		SEL(r).op = SELECTION_CONT;
	    break;
	case SELECTION_DONE:
	    SEL(r).op = SELECTION_CONT;
	    /* FALLTHROUGH */
	case SELECTION_CONT:
	    break;
	case SELECTION_CLEAR:
	    rxvt_selection_start_colrow(r, page, col, row);
	    /* FALLTHROUGH */
	default:
	    return;
    }

    if (
	  SEL(r).beg.col == SEL(r).end.col
	  && SEL(r).beg.col != SEL(r).mark.col
	  && SEL(r).beg.row == SEL(r).end.row
	  && SEL(r).beg.row != SEL(r).mark.row
       )
    {
	SEL(r).beg.col = SEL(r).end.col = SEL(r).mark.col;
	SEL(r).beg.row = SEL(r).end.row = SEL(r).mark.row;
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN,
	    "rxvt_selection_extend_colrow %d () "
	    "ENT2 b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)\n",
	    page, SEL(r).beg.row, SEL(r).beg.col, SEL(r).mark.row,
	    SEL(r).mark.col, SEL(r).end.row, SEL(r).end.col));
    }

    pos.col = col;
    pos.row = row;

    pos.row -= VSTART;	/* adjust for scroll */


#ifndef NO_OLD_SELECTION
    /*
    ** This mimics some of the selection behaviour of version 2.20
    ** and before.
    ** There are no ``selection modes'', button3 is always character
    ** extension.
    ** Note: button3 drag is always available, c.f. v2.20
    ** Selection always terminates (left or right as appropriate) at
    ** the mark.
    */
    if (r->selection_style == OLD_SELECT)
    {
	if (SEL(r).clicks == 1 || button3)
	{
	    if (r->h->hate_those_clicks)
	    {
		r->h->hate_those_clicks = 0;
		if (SEL(r).clicks == 1)
		{
		    SEL(r).beg.row = SEL(r).mark.row;
		    SEL(r).beg.col = SEL(r).mark.col;
		}
		else
		{
		    SEL(r).mark.row = SEL(r).beg.row;
		    SEL(r).mark.col = SEL(r).beg.col;
		}
	    }

	    if (RC_BEFORE(pos, SEL(r).mark))
	    {
		SEL(r).end.row = SEL(r).mark.row;
		SEL(r).end.col = SEL(r).mark.col + 1;
		SEL(r).beg.row = pos.row;
		SEL(r).beg.col = pos.col;
	    }
	    else
	    {
		SEL(r).beg.row = SEL(r).mark.row;
		SEL(r).beg.col = SEL(r).mark.col;
		SEL(r).end.row = pos.row;
		SEL(r).end.col = pos.col + 1;
	    }
# ifdef MULTICHAR_SET
	    rxvt_selection_adjust_kanji(r, page);
# endif		    /* MULTICHAR_SET */
	}
	else if (SEL(r).clicks == 2)
	{
	    rxvt_selection_delimit_word(r, page, UP, &(SEL(r).mark),
		&(SEL(r).beg));
	    rxvt_selection_delimit_word(r, page, DN, &(SEL(r).mark),
		&(SEL(r).end));
	    r->h->hate_those_clicks = 1;
	}
	else if (SEL(r).clicks == 3)
	{
	    SEL(r).beg.row = SEL(r).end.row = SEL(r).mark.row;
	    SEL(r).beg.col = 0;
	    SEL(r).end.col = ncol;
	    r->h->hate_those_clicks = 1;
	}

	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_extend_colrow %d () EXIT b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)\n", page, SEL(r).beg.row, SEL(r).beg.col, SEL(r).mark.row, SEL(r).mark.col, SEL(r).end.row, SEL(r).end.col));
	return;
    }
#endif		    /* ! NO_OLD_SELECTION */


#ifndef NO_NEW_SELECTION
    /* selection_style must not be OLD_SELECT to get here */
    /*
    ** This is mainly xterm style selection with a couple of
    ** differences, mainly in the way button3 drag extension
    ** works.
    ** We're either doing: button1 drag; button3 press; or
    ** button3 drag
    **  a) button1 drag : select around a midpoint/word/line -
    ** that point/word/line is always at the left/right edge
    ** of the SEL(r).
    **  b) button3 press: extend/contract character/word/line
    ** at whichever edge of the selection we are closest to.
    **  c) button3 drag : extend/contract character/word/line
    ** - we select around a point/word/line which is either
    ** the start or end of the selection and it was decided
    ** by whichever point/word/line was `fixed' at the time
    ** of the most recent button3 press
    */
    if (button3 && buttonpress)	    /* button3 press */
    {
	/* first determine which edge of the selection we are
	** closest to
	*/
	if (RC_BEFORE(pos, SEL(r).beg) ||
	    (!RC_AFTER(pos, SEL(r).end) &&
	     (((pos.col - SEL(r).beg.col) +
	       ((pos.row - SEL(r).beg.row) * ncol)) <
	      ((SEL(r).end.col - pos.col) +
	       ((SEL(r).end.row - pos.row) * ncol)))))
	     closeto = LEFT;

	if (closeto == LEFT)
	{
	    SEL(r).beg.row = pos.row;
	    SEL(r).beg.col = pos.col;
	    SEL(r).mark.row = SEL(r).end.row;
	    SEL(r).mark.col = SEL(r).end.col
		    - (SEL(r).clicks == 2);
	}
	else
	{
	    SEL(r).end.row = pos.row;
	    SEL(r).end.col = pos.col;
	    SEL(r).mark.row = SEL(r).beg.row;
	    SEL(r).mark.col = SEL(r).beg.col;
	}
    }
    else	    /* button1 drag or button3 drag */
    {
	if (RC_AFTER(SEL(r).mark, pos))
	{
	    if ((SEL(r).mark.row == SEL(r).end.row) &&
		(SEL(r).mark.col == SEL(r).end.col) &&
		clickchange && SEL(r).clicks == 2)
		SEL(r).mark.col--;
	    SEL(r).beg.row = pos.row;
	    SEL(r).beg.col = pos.col;
	    SEL(r).end.row = SEL(r).mark.row;
	    SEL(r).end.col = SEL(r).mark.col
	       + (SEL(r).clicks == 2);
	}
	else
	{
	    SEL(r).beg.row = SEL(r).mark.row;
		SEL(r).beg.col = SEL(r).mark.col;
		SEL(r).end.row = pos.row;
		SEL(r).end.col = pos.col;
	}
    }


    if (SEL(r).clicks == 1)
    {
	end_col = PSCR(r, page).tlen[SEL(r).beg.row + SVLINES];
	if (end_col != -1 && SEL(r).beg.col > end_col)
	{
#if 1
	    SEL(r).beg.col = ncol;
#else
	    if (SEL(r).beg.row != SEL(r).end.row)
		SEL(r).beg.col = ncol;
	    else
		SEL(r).beg.col = SEL(r).mark.col;
#endif
	}
	end_col = PSCR(r, page).tlen[SEL(r).end.row +
		    SVLINES];
	if (end_col != -1 && SEL(r).end.col > end_col)
	    SEL(r).end.col = ncol;

# ifdef MULTICHAR_SET
	rxvt_selection_adjust_kanji(r, page);
# endif		    /* MULTICHAR_SET */
    }
    else if (SEL(r).clicks == 2)
    {
	if (RC_AFTER(SEL(r).end, SEL(r).beg))
	    SEL(r).end.col--;
	rxvt_selection_delimit_word(r, page, UP, &(SEL(r).beg),
	    &(SEL(r).beg));
	rxvt_selection_delimit_word(r, page, DN, &(SEL(r).end),
	    &(SEL(r).end));
    }
    else if (SEL(r).clicks == 3)
    {
#ifndef NO_FRILLS
	if (ISSET_OPTION(r, Opt_tripleclickwords))
	{
	    int		 end_row;

	    rxvt_selection_delimit_word(r, page, UP, &(SEL(r).beg),
		&(SEL(r).beg));
	    end_row = PSCR(r, page).tlen[SEL(r).mark.row +
			SVLINES];
	    for (
		  end_row = SEL(r).mark.row;
		  end_row < r->TermWin.nrow;
		  end_row++
		)
	    {
		end_col = PSCR(r, page).tlen[end_row + SVLINES];
		if (end_col != -1)
		{
		    SEL(r).end.row = end_row;
		    SEL(r).end.col = end_col;
		    rxvt_selection_trim(r, page);
		    break;
		}
	    }	/* for */
	}
	else
#endif
	{
	    if (RC_AFTER(SEL(r).mark, SEL(r).beg))
		SEL(r).mark.col++;
	    SEL(r).beg.col = 0;
	    SEL(r).end.col = ncol;
	}
    }	/* if (ISSET_OPTION(r, Opt_tripleclickwords)) */

    if (button3 && buttonpress)
    {
	/* mark may need to be changed */
	if (closeto == LEFT)
	{
	    SEL(r).mark.row = SEL(r).end.row;
	    SEL(r).mark.col = SEL(r).end.col - (SEL(r).clicks == 2);
	}
	else
	{
	    SEL(r).mark.row = SEL(r).beg.row;
	    SEL(r).mark.col = SEL(r).beg.col;
	}
    }

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_selection_extend_colrow %d () EXIT b:(r:%d,c:%d) m:(r:%d,c:%d), e:(r:%d,c:%d)\n", page, SEL(r).beg.row, SEL(r).beg.col, SEL(r).mark.row, SEL(r).mark.col, SEL(r).end.row, SEL(r).end.col));