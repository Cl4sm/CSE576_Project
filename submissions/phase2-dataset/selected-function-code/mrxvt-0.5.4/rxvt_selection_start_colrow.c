 * Mark a selection at the specified col/row
 */
/* INTPROTO */
void
rxvt_selection_start_colrow(rxvt_t* r, int page, int col, int row)
{
    PVTS(r, page)->want_refresh = 1;
    SEL(r).mark.col = col;
    SEL(r).mark.row = row - VSTART;
    MAX_IT(SEL(r).mark.row, -(int32_t)PVTS(r, page)->nscrolled);
    MIN_IT(SEL(r).mark.row, (int32_t)r->TermWin.nrow - 1);
    MAX_IT(SEL(r).mark.col, 0);
    MIN_IT(SEL(r).mark.col, (int32_t)r->TermWin.ncol - 1);

    if (SEL(r).op)
    {
	/* clear the old selection */
	SEL(r).beg.row = SEL(r).end.row = SEL(r).mark.row;
	SEL(r).beg.col = SEL(r).end.col = SEL(r).mark.col;
    }
    SEL(r).op = SELECTION_INIT;