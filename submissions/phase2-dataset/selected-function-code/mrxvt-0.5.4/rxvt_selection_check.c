 * -PVTS(r, page)->nscrolled <= (selection row) <= r->TermWin.nrow - 1
 */
/* EXTPROTO */
void
rxvt_selection_check(rxvt_t* r, int page, int check_more)
{
    row_col_t	   pos;

    if (!SEL(r).op ||
	SEL(r).vt != page ||
	SEL(r).screen != PVTS(r, page)->current_screen)
	return;

    pos.row = pos.col = 0;
    if (
	    (SEL(r).beg.row < -(int32_t)PVTS(r, page)->nscrolled) ||
	    (SEL(r).beg.row >= r->TermWin.nrow) ||
	    (SEL(r).mark.row < -(int32_t)PVTS(r, page)->nscrolled) ||
	    (SEL(r).mark.row >= r->TermWin.nrow) ||
	    (SEL(r).end.row < -(int32_t)PVTS(r, page)->nscrolled) ||
	    (SEL(r).end.row >= r->TermWin.nrow) ||
	    ( check_more == 1 &&
	      PVTS(r, page)->current_screen == SEL(r).screen &&
	      !RC_BEFORE(PSCR(r, page).cur, SEL(r).beg) &&
	      RC_BEFORE(PSCR(r, page).cur, SEL(r).end)) ||
	    ( check_more == 2 &&
	      RC_BEFORE(SEL(r).beg, pos) &&
	      RC_AFTER(SEL(r).end, pos)) ||
	    ( check_more == 3 &&
	      RC_AFTER(SEL(r).end, pos)) ||
	    ( check_more == 4	/* screen width change */ &&
	      ( SEL(r).beg.row != SEL(r).end.row ||
		SEL(r).end.col > r->TermWin.ncol))
       )
    {