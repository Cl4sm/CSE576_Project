void
scr_release(void)
{
    int             i, total_rows;

    total_rows = TermWin.nrow + TermWin.saveLines;
    for (i = 0; i < total_rows; i++) {
	if (screen.text[i]) {	/* then so is screen.rend[i] */
	    FREE(screen.text[i]);
	    FREE(screen.rend[i]);
	}
    }
    for (i = 0; i < TermWin.nrow; i++) {
	FREE(drawn_text[i]);
	FREE(drawn_rend[i]);
	FREE(swap.text[i]);
	FREE(swap.rend[i]);
    }
    FREE(screen.text);
    FREE(screen.tlen);
    FREE(screen.rend);
    FREE(drawn_text);
    FREE(drawn_rend);
    FREE(swap.text);
    FREE(swap.tlen);
    FREE(swap.rend);
    FREE(buf_text);
    FREE(buf_tlen);
    FREE(buf_rend);
    FREE(tabs);

/* NULL these so if anything tries to use them, we'll know about it */
    screen.text = drawn_text = swap.text = NULL;
    screen.rend = drawn_rend = swap.rend = NULL;
    screen.tlen = swap.tlen = buf_tlen = NULL;
    buf_text = NULL;
    buf_rend = NULL;
    tabs = NULL;
}
