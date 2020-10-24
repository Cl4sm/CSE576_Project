void
rxvt_scr_alloc (rxvt_t* r, int page)
{
    unsigned int    ncol, nrow, total_rows;
    unsigned int    p, q;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_alloc %d ()\n", page));
    ncol = r->TermWin.ncol;
    nrow = r->TermWin.nrow;
    total_rows = nrow + SVLINES;

    /*
    ** First time called so just malloc everything : don't rely on
    ** realloc
    ** Note: this is still needed so that all the scrollback lines
    ** are NULL
    */
    PVTS(r, page)->buf_text = rxvt_calloc(total_rows, sizeof(text_t*));
    PVTS(r, page)->buf_rend = rxvt_calloc(total_rows, sizeof(rend_t*));

    PVTS(r, page)->drawn_text = rxvt_calloc(nrow, sizeof(text_t*));
    PVTS(r, page)->drawn_rend = rxvt_calloc(nrow, sizeof(rend_t*));

    PSCR(r, page).text = rxvt_calloc(total_rows, sizeof(text_t*));
    PSCR(r, page).tlen = rxvt_calloc(total_rows, sizeof(int16_t));
    PSCR(r, page).rend = rxvt_calloc(total_rows, sizeof(rend_t*));

#if NSCREENS
    PVTS(r, page)->swap.text = rxvt_calloc(nrow, sizeof(text_t*));
    PVTS(r, page)->swap.tlen = rxvt_calloc(nrow, sizeof(int16_t));
    PVTS(r, page)->swap.rend = rxvt_calloc(nrow, sizeof(rend_t*));
#endif

    for (p = 0; p < nrow; p++)
    {
	q = p + SVLINES;
	rxvt_blank_screen_mem (r, page, PSCR(r, page).text,
	    PSCR(r, page).rend, q, DEFAULT_RSTYLE);
	PSCR(r, page).tlen[q] = 0;
#if NSCREENS
	rxvt_blank_screen_mem (r, page, PVTS(r, page)->swap.text,
	    PVTS(r, page)->swap.rend, p, DEFAULT_RSTYLE);
	PVTS(r, page)->swap.tlen[p] = 0;
#endif

	rxvt_blank_screen_mem (r, page, PVTS(r, page)->drawn_text,
	    PVTS(r, page)->drawn_rend, p, DEFAULT_RSTYLE);
    }
    PVTS(r, page)->nscrolled = 0;   /* no saved lines */
    PSCR(r, page).flags = Screen_DefaultFlags;
    PSCR(r, page).cur.row = 0;
    PSCR(r, page).cur.col = 0;
    PSCR(r, page).charset = 0;
    PVTS(r, page)->current_screen = PRIMARY;
    rxvt_scr_cursor(r, page, SAVE);
#if NSCREENS
    PVTS(r, page)->swap.flags = Screen_DefaultFlags;
    PVTS(r, page)->swap.cur.row = 0;
    PVTS(r, page)->swap.cur.col = 0;
    PVTS(r, page)->swap.charset = 0;
    PVTS(r, page)->current_screen = SECONDARY;
    rxvt_scr_cursor(r, page, SAVE);
    PVTS(r, page)->current_screen = PRIMARY;
#endif

    PVTS(r, page)->rstyle = DEFAULT_RSTYLE;
    PVTS(r, page)->rvideo = 0;
    MEMSET(&(PVTS(r, page)->charsets), 'B', sizeof(PVTS(r, page)->charsets));
#ifdef MULTICHAR_SET
    PVTS(r, page)->multi_byte = 0;
    PVTS(r, page)->lost_multi = 0;
    PVTS(r, page)->chstat = SBYTE;
#endif

    /* Now set screen initialization flag */
    PVTS(r, page)->init_screen = 1;
}