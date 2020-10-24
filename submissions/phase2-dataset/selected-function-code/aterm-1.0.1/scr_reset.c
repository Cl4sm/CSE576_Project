scr_reset(void)
{
    int             i, j, k, total_rows, prev_total_rows;
    rend_t	    setrstyle;

    D_SCREEN((stderr, "scr_reset()"));

    TermWin.view_start = 0;
    RESET_CHSTAT;

    if (TermWin.bcol == prev_bcol && TermWin.nrow == prev_nrow)
	return;
#if 0 
#ifdef DEBUG_STRICT
    assert(TermWin.saveLines >= 0);
#else				/* drive with your eyes closed */
    MAX_IT(TermWin.saveLines, 0);
#endif
#endif 

    total_rows = TermWin.nrow + TermWin.saveLines;
    prev_total_rows = prev_nrow + TermWin.saveLines;

    screen.tscroll = 0;
    screen.bscroll = (TermWin.nrow - 1);
/* *INDENT-OFF* */
    if (prev_nrow == -1) {
/*
 * A: first time called so just malloc everything : don't rely on realloc
 *    Note: this is still needed so that all the scrollback lines are NULL
 */
	screen.text	= CALLOC(text_t * , total_rows  );
	screen.tlen	= CALLOC(R_int16_t, total_rows  );
	buf_text	= CALLOC(text_t * , total_rows  );
	buf_tlen	= CALLOC(R_int16_t, total_rows  );
	drawn_text	= CALLOC(text_t * , TermWin.nrow);
	swap.text	= CALLOC(text_t * , TermWin.nrow);
	swap.tlen	= CALLOC(R_int16_t, TermWin.nrow);

	screen.rend = CALLOC(rend_t *, total_rows  );
	buf_rend    = CALLOC(rend_t *, total_rows  );
	drawn_rend  = CALLOC(rend_t *, TermWin.nrow);
	swap.rend   = CALLOC(rend_t *, TermWin.nrow);

	for (i = 0; i < TermWin.nrow; i++) {
	    j = i + TermWin.saveLines;
	    make_screen_mem(screen.text, screen.rend, j, DEFAULT_RSTYLE);
	    make_screen_mem(swap.text, swap.rend, i, DEFAULT_RSTYLE);
	    make_screen_mem(drawn_text, drawn_rend, i, DEFAULT_RSTYLE );
	    screen.tlen[j] = swap.tlen[i] = 0;
	}
	TermWin.nscrolled = 0;	/* no saved lines */
	screen.flags = swap.flags = Screen_DefaultFlags;
	save.cur.row = save.cur.col = 0;
	save.charset = 0;
	save.charset_char = 'B';
	rstyle = save.rstyle = DEFAULT_RSTYLE;
	selection.text = NULL;
	selection.op = SELECTION_CLEAR;
	selection.screen = PRIMARY;
	selection.clicks = 0;
	CLEAR_ALL_SELECTION;
	MEMSET(charsets, 'B', sizeof(charsets));
	current_screen = PRIMARY;
	rvideo = 0;
#ifdef MULTICHAR_SET
	multi_byte = 0;
	lost_multi = 0;
	chstat = SBYTE;
# ifdef KANJI
	encoding_method = EUCJ;
# else
	encoding_method = BIG5;
# endif
#endif

    } else {
/*
 * B1: add or delete rows as appropriate
 */
	setrstyle = DEFAULT_RSTYLE | (rvideo ? RS_RVid : 0);

	if ((int)TermWin.nrow < prev_nrow) {
	/* delete rows */
	    k = min((int)TermWin.nscrolled, prev_nrow - (int)TermWin.nrow);
	    scroll_text(0, prev_nrow - 1, k, 1);
	    for (i = TermWin.nrow; i < prev_nrow; i++) {
		j = i + TermWin.saveLines;
		if (screen.text[j])
		    FREE(screen.text[j]);
		if (screen.rend[j])
		    FREE(screen.rend[j]);
		if (swap.text[i])
		    FREE(swap.text[i]);
		if (swap.rend[i])
		    FREE(swap.rend[i]);
		FREE(drawn_text[i]);
		FREE(drawn_rend[i]);
	    }
	/* we have fewer rows so fix up number of scrolled lines */
	    MIN_IT(screen.cur.row, TermWin.nrow - 1);
	    MIN_IT(swap.cur.row, TermWin.nrow - 1);
	}
	if(TermWin.nrow != prev_nrow) {
	    screen.text = REALLOC(screen.text, total_rows  *sizeof(text_t*));
	    screen.tlen = REALLOC(screen.tlen, total_rows  *sizeof(R_int16_t));
	    buf_text  	= REALLOC(buf_text   , total_rows  *sizeof(text_t*));
	    buf_tlen  	= REALLOC(buf_tlen   , total_rows  *sizeof(R_int16_t));
	    drawn_text  = REALLOC(drawn_text , TermWin.nrow*sizeof(text_t*));
	    swap.text  	= REALLOC(swap.text  , TermWin.nrow*sizeof(text_t*));
	    swap.tlen   = REALLOC(swap.tlen  , total_rows  *sizeof(R_int16_t));

	    screen.rend = REALLOC(screen.rend, total_rows  *sizeof(rend_t*));
	    buf_rend  	= REALLOC(buf_rend   , total_rows  *sizeof(rend_t*));
	    drawn_rend  = REALLOC(drawn_rend , TermWin.nrow*sizeof(rend_t*));
	    swap.rend  	= REALLOC(swap.rend  , TermWin.nrow*sizeof(rend_t*));
	}
	if ((int)TermWin.nrow > prev_nrow) {
	/* add rows */
	    k = min((int)TermWin.nscrolled, (int)TermWin.nrow - prev_nrow);
	    for (i = prev_total_rows; i < total_rows - k; i++) {
		make_screen_mem(screen.text, screen.rend, i, setrstyle);
		screen.tlen[i] = 0;
	    }
	    for (i = total_rows - k; i < total_rows; i++) {
		screen.text[i] = NULL;
		screen.rend[i] = NULL;
		screen.tlen[i] = 0;
	    }
	    for (i = prev_nrow; i < TermWin.nrow; i++) {
		make_screen_mem(swap.text, swap.rend, i, setrstyle);
	        make_screen_mem(drawn_text, drawn_rend, i, setrstyle );
		swap.tlen[i] = 0;
	    }

	    if (k > 0) {
		scroll_text(0, TermWin.nrow - 1, -k, 1);
		screen.cur.row += k;
		TermWin.nscrolled -= k;
		for (i = TermWin.saveLines - TermWin.nscrolled; k--; i--)
		    if (screen.text[i] == NULL) {
			make_screen_mem(screen.text, screen.rend, i,
					setrstyle);
			screen.tlen[i] = 0;
		    }
	    }
#ifdef DEBUG_STRICT
	    assert(screen.cur.row < TermWin.nrow);
	    assert(swap.cur.row < TermWin.nrow);
#else				/* drive with your eyes closed */
	    MIN_IT(screen.cur.row, TermWin.nrow - 1);
	    MIN_IT(swap.cur.row, TermWin.nrow - 1);
#endif
	}

/* B2: resize columns */
	if (TermWin.bcol != prev_bcol) {
	    for (i = 0; i < total_rows; i++) {
		if (screen.text[i]) {
		    screen.text[i] = REALLOC(screen.text[i],
					      TermWin.bcol * sizeof(text_t));
		    screen.rend[i] = REALLOC(screen.rend[i],
					      TermWin.bcol * sizeof(rend_t));
		    MIN_IT(screen.tlen[i], TermWin.bcol);
		    if (TermWin.bcol > prev_bcol)
			blank_line(&(screen.text[i][prev_bcol]),
				   &(screen.rend[i][prev_bcol]),
				   TermWin.bcol - prev_bcol, setrstyle);
		}
	    }
	    for (i = 0; i < TermWin.nrow; i++) {
		drawn_text[i] = REALLOC(drawn_text[i],
					 TermWin.bcol * sizeof(text_t));
		drawn_rend[i] = REALLOC(drawn_rend[i],
					 TermWin.bcol * sizeof(rend_t));
		if (swap.text[i]) {
		    swap.text[i] = REALLOC(swap.text[i],
					    TermWin.bcol * sizeof(text_t));
		    swap.rend[i] = REALLOC(swap.rend[i],
					    TermWin.bcol * sizeof(rend_t));
		    MIN_IT(swap.tlen[i], TermWin.bcol);
		    if (TermWin.bcol > prev_bcol)
			blank_line(&(swap.text[i][prev_bcol]),
				   &(swap.rend[i][prev_bcol]),
				   TermWin.bcol - prev_bcol, setrstyle);
		}
		if (TermWin.bcol > prev_bcol)
		    blank_line(&(drawn_text[i][prev_bcol]),
			       &(drawn_rend[i][prev_bcol]),
			       TermWin.bcol - prev_bcol, setrstyle);
	    }
	    MIN_IT(screen.cur.col, TermWin.bcol - 1);
	    MIN_IT(swap.cur.col, TermWin.bcol - 1);
	}
	if (tabs)
	    FREE(tabs);
    }
/* *INDENT-ON* */

    tabs = MALLOC(TermWin.bcol * sizeof(char));

    for (i = 0; i < TermWin.bcol; i++)
	tabs[i] = (i % TABSIZE == 0) ? 1 : 0;

    prev_nrow = TermWin.nrow;
    prev_bcol = TermWin.bcol;

    tt_resize();
}
