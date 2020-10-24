
/* EXTPROTO */
void
rxvt_scr_expose(rxvt_t* r, int page,
	int x, int y, int width, int height,
	Bool refresh)
{
    int		 i;
    row_col_t	   rc[RC_COUNT];

    if (PVTS(r, page)->drawn_text == NULL)  /* sanity check */
	return;

    x = max(x, (int)r->TermWin.int_bwidth);
    x = min(x, (int)r->szHint.width);
    y = max(y, (int)r->TermWin.int_bwidth);
    y = min(y, (int)r->szHint.height);

    /* round down */
    rc[PART_BEG].col = Pixel2Col(x);
    rc[PART_BEG].row = Pixel2Row(y);
    /* round up */
    rc[PART_END].col = Pixel2Width(x + width + r->TermWin.fwidth - 1);
    rc[PART_END].row = Pixel2Row(y + height + r->TermWin.fheight - 1);

    /* sanity checks */
    for (i = PART_BEG; i < RC_COUNT; i++)
    {
	MIN_IT(rc[i].col, r->TermWin.ncol - 1);
	MIN_IT(rc[i].row, r->TermWin.nrow - 1);
    }

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_expose %d (x:%d, y:%d, w:%d, h:%d) area (c:%d,r:%d)-(c:%d,r:%d)\n", page, x, y, width, height, rc[PART_BEG].col, rc[PART_BEG].row, rc[PART_END].col, rc[PART_END].row));

	{
		register int	j = rc[PART_BEG].col;
		register int	k = rc[PART_END].col - rc[PART_BEG].col + 1;

		for (i = rc[PART_BEG].row; i <= rc[PART_END].row; i++)
		{
			rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, " memset drawn_text[%d][%d], len=%d\n", i, j, k));
			MEMSET(&(PVTS(r, page)->drawn_text[i][j]), 0, k);
		}
	 }

    if (refresh)
    {
	rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "Forcing immediate screen refresh"));
	rxvt_scr_refresh(r, page, SLOW_REFRESH | REFRESH_BOUNDS);