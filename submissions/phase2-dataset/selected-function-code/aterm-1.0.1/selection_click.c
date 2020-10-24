selection_click(int clicks, int x, int y)
{
/*    int             r, c;
 *   row_col_t             ext_beg, ext_end;
 */

    D_SELECT((stderr, "selection_click(%d, %d, %d)", clicks, x, y));

    clicks = ((clicks - 1) % 3) + 1;
    selection.clicks = clicks;	/* save clicks so extend will work */
#ifdef THAI
    selection_start_colrow(ThaiPixel2Col(x,y), Pixel2Row(y));
#else
    selection_start_colrow(Pixel2Col(x), Pixel2Row(y));
#endif
    if (clicks == 2 || clicks == 3)
	selection_extend_colrow(selection.mark.col,
				selection.mark.row + TermWin.view_start,
				0,	/* button 3     */
				1,	/* button press */
				0);	/* click change */
}
