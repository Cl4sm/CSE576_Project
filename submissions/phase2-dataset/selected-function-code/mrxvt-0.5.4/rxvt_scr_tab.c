void
rxvt_scr_tab(rxvt_t* r, int page, int count)
{
    int		 i, x;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_tab %d (%d)\n", page, count));
    PVTS(r, page)->want_refresh = 1;
    RESET_CHSTAT(r, page);
    i = x = CURCOL;
    if (count == 0)
	return;
    else if (count > 0)
    {
	for (; ++i < r->TermWin.ncol; )
	    if (r->tabstop[i])
	    {
		x = i;
		if (!--count)
		    break;
	    }
	;
	if (count)
	    x = r->TermWin.ncol - 1;
    }
    else /* if (count < 0) */
    {
	for (; --i >= 0; )
	    if (r->tabstop[i])
	    {
		x = i;
		if (!++count)
		    break;
	    }
	;
	if (count)
	    x = 0;
    }

#if 0
    if (x != CURCOL)
	rxvt_scr_gotorc(r, page, 0, x, R_RELATIVE);
#else
    /*
     * 2006-09-02 gi1242: Don't call rxvt_scr_gotorc() because that might change
     * CURROW (if it was negative). If we're adding lines to the screen
     * structure, then CURROW is allowed to be negative.
     */
    CURCOL = x;
#endif
}