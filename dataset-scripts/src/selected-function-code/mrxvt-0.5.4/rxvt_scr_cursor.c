void
rxvt_scr_cursor(rxvt_t* r, int page, int mode)
{
    screen_t	   *s;

    rxvt_dbgmsg ((DBG_VERBOSE, DBG_SCREEN, "rxvt_scr_cursor %d (%c)\n", page, mode));

#if NSCREENS && !defined(NO_SECONDARY_SCREEN_CURSOR)
    if (PVTS(r, page)->current_screen == SECONDARY)
	s = &(PVTS(r, page)->swap);
    else
#endif
    s = &(PSCR(r, page));
    switch (mode)
    {
	case SAVE:
	    s->s_cur.row = s->cur.row;
	    s->s_cur.col = s->cur.col;
	    s->s_rstyle = PVTS(r, page)->rstyle;
	    s->s_charset = s->charset;
	    s->s_charset_char = PVTS(r, page)->charsets[s->charset];
	    break;
	case RESTORE:
	    PVTS(r, page)->want_refresh = 1;
	    s->cur.row = s->s_cur.row;
	    s->cur.col = s->s_cur.col;
	    s->flags &= ~Screen_WrapNext;
	    PVTS(r, page)->rstyle = s->s_rstyle;
	    s->charset = s->s_charset;
	    PVTS(r, page)->charsets[s->charset] = s->s_charset_char;
	    rxvt_set_font_style(r, page);
	    break;
    }
/* boundary check in case screen size changed between SAVE and RESTORE */
    MIN_IT(s->cur.row, r->TermWin.nrow - 1);
    MIN_IT(s->cur.col, r->TermWin.ncol - 1);
    assert(s->cur.row >= 0);
    assert(s->cur.col >= 0);
    MAX_IT(s->cur.row, 0);
    MAX_IT(s->cur.col, 0);
}