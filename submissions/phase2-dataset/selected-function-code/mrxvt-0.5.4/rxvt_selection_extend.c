 * flag == 2 ==> button 3 motion
 */
/* EXTPROTO */
void
rxvt_selection_extend(rxvt_t* r, int page, int x, int y, int flag)
{
    int		 col, row;

    col = Pixel2Col(x);
    row = Pixel2Row(y);
    MAX_IT(row, 0);
    MIN_IT(row, (int)r->TermWin.nrow - 1);
    MAX_IT(col, 0);
    MIN_IT(col, (int)r->TermWin.ncol);

#ifndef NO_NEW_SELECTION
/*
 * If we're selecting characters (single click) then we must check first
 * if we are at the same place as the original mark.  If we are then
 * select nothing.  Otherwise, if we're to the right of the mark, you have to
 * be _past_ a character for it to be selected.
 */
    if (r->selection_style != OLD_SELECT)
    {
	if (
	      ((SEL(r).clicks % 3) == 1) && !flag
	      && (
		    col == SEL(r).mark.col
		    && (row == SEL(r).mark.row + VSTART)
		 )
	   )
	{
	    /* select nothing */
	    SEL(r).beg.row = SEL(r).end.row = 0;
	    SEL(r).beg.col = SEL(r).end.col = 0;
	    SEL(r).clicks = 4;
	    PVTS(r, page)->want_refresh = 1;
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_extend %d () sel.clicks = 4\n", page));
	    return;
	}
    }
#endif
    if (SEL(r).clicks == 4)
	SEL(r).clicks = 1;
    rxvt_selection_extend_colrow(r, page, col, row, !!flag,
	/* ? button 3	  */