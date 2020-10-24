void
selection_extend(int x, int y, int flag)
{
    int             col, row;

#ifdef THAI
    col = ThaiPixel2Col( x, y);
#else
    col = Pixel2Col(x);
#endif
    row = Pixel2Row(y);
    MAX_IT(row, 0);
    MIN_IT(row, TermWin.nrow - 1);
    MAX_IT(col, 0);
    MIN_IT(col, TermWin.bcol);
#ifndef OLD_SELECTION
/*
 * If we're selecting characters (single click) then we must check first
 * if we are at the same place as the original mark.  If we are then
 * select nothing.  Otherwise, if we're to the right of the mark, you have to
 * be _past_ a character for it to be selected.
 */
    if (((selection.clicks % 3) == 1) && !flag
	&& (col == selection.mark.col
	    && (row == selection.mark.row + TermWin.view_start))) {
    /* select nothing */
	selection.beg.row = selection.end.row = 0;
	selection.beg.col = selection.end.col = 0;
	selection.clicks = 4;
	D_SELECT((stderr, "selection_extend() selection.clicks = 4"));
	return;
    }
#endif
    if (selection.clicks == 4)
	selection.clicks = 1;
    selection_extend_colrow(col, row,
			    !!flag,	/* ? button 3      */
			    flag == 1 ? 1 : 0,	/* ? button press  */
			    0);	/* no click change */
}
