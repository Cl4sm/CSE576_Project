void
resize_window1(unsigned int width, unsigned int height)
{
    static short    first_time = 1;
    int             new_ncol = (width - szHint.base_width) / TermWin.fwidth;
    int             new_nrow = (height - szHint.base_height) / TermWin.fheight;

    if (first_time ||
	(new_ncol != TermWin.ncol) ||
	(new_nrow != TermWin.nrow)) {
	int             curr_screen = -1;

    /* scr_reset only works on the primary screen */
	if (!first_time) {	/* this is not the first time thru */
	    selection_clear();
	    curr_screen = scr_change_screen(PRIMARY);
	}

	set_terminal_size( new_ncol, new_nrow);

	resize_subwindows(width, height);
	scr_reset();

	if (curr_screen >= 0)	/* this is not the first time thru */
	    scr_change_screen(curr_screen);
	first_time = 0;
    }
}
