
#ifndef NO_FRILLS
/* INTPROTO */
void
rxvt_selection_trim(rxvt_t* r, int page)
{
    int32_t	 end_col, end_row;
    text_t	 *stp; 

    end_col = SEL(r).end.col;
    end_row = SEL(r).end.row;
    for ( ; end_row >= SEL(r).beg.row; )
    {
	stp = PSCR(r, page).text[end_row + SVLINES];
	while (--end_col >= 0)
	{
	    if (stp[end_col] != ' ' && stp[end_col] != '\t')
		break;
	}
	if (end_col >= 0 || PSCR(r, page).tlen[end_row - 1 + SVLINES] != -1)
	{
	    SEL(r).end.col = end_col + 1;
	    SEL(r).end.row = end_row;
	    break;
	}
	end_row--;
	end_col = r->TermWin.ncol;
    }

    if (SEL(r).mark.row > SEL(r).end.row)
    {
	SEL(r).mark.row = SEL(r).end.row;
	SEL(r).mark.col = SEL(r).end.col;
    }
    else if (SEL(r).mark.row == SEL(r).end.row &&