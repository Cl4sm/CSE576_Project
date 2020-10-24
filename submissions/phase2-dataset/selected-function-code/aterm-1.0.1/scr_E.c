scr_E(void)
{
    int             i, j;
    text_t         *t;
    rend_t         *r, fs;

    ZERO_SCROLLBACK;
    RESET_CHSTAT;
    CHECK_SELECTION(3);

    fs = rstyle;
    for (i = TermWin.saveLines; i < TermWin.nrow + TermWin.saveLines; i++) {
	t = screen.text[i];
	r = screen.rend[i];
	for (j = 0; j < TermWin.bcol; j++) {
	    *t++ = 'E';
	    *r++ = fs;
	}
	screen.tlen[i] = TermWin.bcol;	/* make the `E's selectable */
    }
}
