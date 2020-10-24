void
rxvt_scr_reset_realloc(rxvt_t* r, int page)
{
    unsigned int   total_rows, nrow;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_reset_realloc %d ()\n", page));
    nrow = r->TermWin.nrow;
    total_rows = nrow + SVLINES;

    PSCR(r, page).text = rxvt_realloc (
	PSCR(r, page).text, total_rows * sizeof(text_t *));
    PSCR(r, page).tlen = rxvt_realloc (
	PSCR(r, page).tlen, total_rows * sizeof(int16_t));
    PSCR(r, page).rend = rxvt_realloc (
	PSCR(r, page).rend, total_rows * sizeof(rend_t *));

#if NSCREENS
    PVTS(r, page)->swap.text   = rxvt_realloc (
	PVTS(r, page)->swap.text, nrow * sizeof(text_t *));
    PVTS(r, page)->swap.tlen   = rxvt_realloc (
	PVTS(r, page)->swap.tlen  , total_rows * sizeof(int16_t));
    PVTS(r, page)->swap.rend   = rxvt_realloc (
	PVTS(r, page)->swap.rend, nrow * sizeof(rend_t *));
#endif

    PVTS(r, page)->buf_text = rxvt_realloc (
	PVTS(r, page)->buf_text, total_rows * sizeof(text_t *));
    PVTS(r, page)->buf_rend = rxvt_realloc (
	PVTS(r, page)->buf_rend, total_rows * sizeof(rend_t *));

    PVTS(r, page)->drawn_text  = rxvt_realloc (
	PVTS(r, page)->drawn_text, nrow * sizeof(text_t *));
    PVTS(r, page)->drawn_rend  = rxvt_realloc (
	PVTS(r, page)->drawn_rend, nrow * sizeof(rend_t *));
}