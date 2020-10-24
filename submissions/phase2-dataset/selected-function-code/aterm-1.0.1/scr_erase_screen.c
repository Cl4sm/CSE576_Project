void
scr_erase_screen(int mode)
{
    int             row, num, row_offset;
    rend_t          ren;
    long            gcmask;
    XGCValues       gcvalue;

    D_SCREEN((stderr, "scr_erase_screen(%d) at screen row: %d", mode, screen.cur.row));
    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    row_offset = TermWin.saveLines;

    switch (mode) {
    case 0:			/* erase to end of screen */
	CHECK_SELECTION(1);
	scr_erase_line(0);
	row = screen.cur.row + 1;	/* possible OOB */
	num = TermWin.nrow - row;
	break;
    case 1:			/* erase to beginning of screen */
	CHECK_SELECTION(3);
	scr_erase_line(1);
	row = 0;		/* possible OOB */
	num = screen.cur.row;
	break;
    case 2:			/* erase whole screen */
	CHECK_SELECTION(3);
	Gr_ClearScreen();
	row = 0;
	num = TermWin.nrow;
	break;
    default:
	return;
    }
    if (selection.op && current_screen == selection.screen
	&& ((selection.beg.row >= row
	     && selection.beg.row <= row + num)
	    || (selection.end.row >= row
		&& selection.end.row <= row + num)))
	CLEAR_SELECTION;
    if (row >= 0 && row < TermWin.nrow) {	/* check OOB */
	MIN_IT(num, (TermWin.nrow - row));
	if (rstyle & (RS_RVid | RS_Uline))
	    ren = (rend_t)~RS_None;
	else if (GET_BGCOLOR(rstyle) == Color_bg) {
	    ren = DEFAULT_RSTYLE;
	    CLEAR_ROWS(row, num);
	} else {
	    ren = (rstyle & (RS_fgMask | RS_bgMask));
	    gcvalue.foreground = PixColors[GET_BGCOLOR(ren)];
	    gcmask = GCForeground;
	    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
	    ERASE_ROWS(row, num);
	    gcvalue.foreground = PixColors[Color_fg];
	    XChangeGC(Xdisplay, TermWin.gc, gcmask, &gcvalue);
	}
	for (; num--; row++) {
	    blank_line(screen.text[row + row_offset],
		       screen.rend[row + row_offset], TermWin.bcol,
		       rstyle & ~RS_Uline);
	    screen.tlen[row + row_offset] = 0;
	    blank_line(drawn_text[row], drawn_rend[row], TermWin.bcol, ren);
	}
    }
}
