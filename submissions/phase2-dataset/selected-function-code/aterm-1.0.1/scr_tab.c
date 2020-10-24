void
scr_tab(int count)
{
    int             i, x;

    RESET_CHSTAT;
    x = screen.cur.col;
    if (count == 0)
	return;
    else if (count > 0) {
	for (i = x + 1; i < TermWin.bcol; i++) {
	    if (tabs[i]) {
		x = i;
		if (!--count)
		    break;
	    }
	}
    } else if (count < 0) {
	for (i = x - 1; i >= 0; i--) {
	    if (tabs[i]) {
		x = i;
		if (!++count)
		    break;
	    }
	}
    }
    if (x != screen.cur.col)
	scr_gotorc(0, x, R_RELATIVE);
}
