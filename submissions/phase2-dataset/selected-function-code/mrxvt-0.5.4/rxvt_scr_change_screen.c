int
rxvt_scr_change_screen(rxvt_t* r, int page, int scrn)
{
#if NSCREENS
    unsigned int	i, offset;
#endif

    PVTS(r, page)->want_refresh = 1;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_change_screen %d (%d)\n", page, scrn));

    VSTART = 0;
    RESET_CHSTAT(r, page);

    if (PVTS(r, page)->current_screen == scrn)
	return PVTS(r, page)->current_screen;

    rxvt_selection_check(r, page, 2);	/* check for boundary cross */

    SWAP_IT(PVTS(r, page)->current_screen, scrn, int);

#if NSCREENS
    PVTS(r, page)->num_scr = 0;
    offset = SVLINES;

    for (i = PVTS(r, page)->prev_nrow; i--;)
    {
	SWAP_IT(PSCR(r, page).text[i + offset],
	    PVTS(r, page)->swap.text[i], text_t *);
	SWAP_IT(PSCR(r, page).tlen[i + offset],
	    PVTS(r, page)->swap.tlen[i], int16_t);
	SWAP_IT(PSCR(r, page).rend[i + offset],
	    PVTS(r, page)->swap.rend[i], rend_t *);
    }
    SWAP_IT(CURROW, PVTS(r, page)->swap.cur.row, int16_t);
    SWAP_IT(CURCOL, PVTS(r, page)->swap.cur.col, int16_t);
    assert (CURROW >= 0);
    assert (CURROW < PVTS(r, page)->prev_nrow);
    assert (CURCOL >= 0);
    assert (CURCOL < PVTS(r, page)->prev_ncol);
    MAX_IT(CURROW, 0);
    MIN_IT(CURROW, (int32_t)PVTS(r, page)->prev_nrow - 1);
    MAX_IT(CURCOL, 0);
    MIN_IT(CURCOL, (int32_t)PVTS(r, page)->prev_ncol - 1);

    SWAP_IT(PSCR(r, page).charset, PVTS(r, page)->swap.charset,
	int16_t);
    SWAP_IT(PSCR(r, page).flags, PVTS(r, page)->swap.flags, int);
    PSCR(r, page).flags |= Screen_VisibleCursor;
    PVTS(r, page)->swap.flags |= Screen_VisibleCursor;

#else
# ifdef SCROLL_ON_NO_SECONDARY
    if (PVTS(r, page)->current_screen == PRIMARY)
	rxvt_scroll_text(r, page, 0, (PVTS(r, page)->prev_nrow - 1),
	    PVTS(r, page)->prev_nrow, 0);
# endif
#endif

    /* Need to update tabbar buttons */
    if (ISSET_OPTION(r, Opt2_protectSecondary))
	rxvt_tabbar_draw_buttons (r);

    return scrn;
}