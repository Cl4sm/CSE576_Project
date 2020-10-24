void
scr_expose(int x, int y, int width, int height)
{
    int             i;
#if 0
    text_t	   *t;
#endif
    row_col_t       full_beg, full_end, part_beg, part_end;

    if (drawn_text == NULL)	/* sanity check */
	return;

/* round down */
    part_beg.col = Pixel2Col(x);
    part_beg.row = Pixel2Row(y);
    full_end.col = Pixel2Width(x + width);
    full_end.row = Pixel2Row(y + height);

/* round up */
    part_end.col = Pixel2Width(x + width + TermWin.fwidth - 1);
    part_end.row = Pixel2Row(y + height + TermWin.fheight - 1);
    full_beg.col = Pixel2Col(x + TermWin.fwidth - 1);
    full_beg.row = Pixel2Row(y + TermWin.fheight - 1);

/* sanity checks */
    MAX_IT(part_beg.col, 0);
    MAX_IT(full_beg.col, 0);
    MAX_IT(part_end.col, 0);
    MAX_IT(full_end.col, 0);
    MAX_IT(part_beg.row, 0);
    MAX_IT(full_beg.row, 0);
    MAX_IT(part_end.row, 0);
    MAX_IT(full_end.row, 0);
    MIN_IT(part_beg.col, TermWin.bcol - 1);
    MIN_IT(full_beg.col, TermWin.bcol - 1);
    MIN_IT(part_end.col, TermWin.bcol - 1);
    MIN_IT(full_end.col, TermWin.bcol - 1);
    MIN_IT(part_beg.row, TermWin.nrow - 1);
    MIN_IT(full_beg.row, TermWin.nrow - 1);
    MIN_IT(part_end.row, TermWin.nrow - 1);
    MIN_IT(full_end.row, TermWin.nrow - 1);

    D_SCREEN((stderr, "scr_expose(x:%d, y:%d, w:%d, h:%d) area (c:%d,r:%d)-(c:%d,r:%d)", x, y, width, height, part_beg.col, part_beg.row, part_end.col, part_end.row));

#if 1	/* XXX: check for backing store/save unders? */
    for (i = part_beg.row; i <= part_end.row; i++)
	    MEMSET(&(drawn_text[i][part_beg.col]), 0,
		   part_end.col - part_beg.col + 1);
#else
     if (full_end.col >= full_beg.col)
     /* set DEFAULT_RSTYLE for totally exposed characters */
       for (i = full_beg.row; i <= full_end.row; i++)
           blank_line(&(drawn_text[i][full_beg.col]),
                      &(drawn_rend[i][full_beg.col]),
                      full_end.col - full_beg.col + 1, DEFAULT_RSTYLE);
/* force an update for partially exposed characters */
    if (part_beg.row != full_beg.row) {
	t = &(drawn_text[part_beg.row][part_beg.col]);
	for (i = part_end.col - part_beg.col + 1; i--;)
	    *t++ = 0;
    }
    if (part_end.row != full_end.row) {
	t = &(drawn_text[part_end.row][part_beg.col]);
	for (i = part_end.col - part_beg.col + 1; i--;)
	    *t++ = 0;
    }
    if (part_beg.col != full_beg.col)
	for (i = full_beg.row; i <= full_end.row; i++)
	    drawn_text[i][part_beg.col] = 0;
    if (part_end.col != full_end.col)
	for (i = full_beg.row; i <= full_end.row; i++)
	    drawn_text[i][part_end.col] = 0;
#endif
    scr_refresh(SLOW_REFRESH);
}
