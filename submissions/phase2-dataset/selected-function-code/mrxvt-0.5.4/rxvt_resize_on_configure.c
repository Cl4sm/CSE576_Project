void
rxvt_resize_on_configure (rxvt_t* r, unsigned int width, unsigned int height)
{
    register int    i = 0;
    unsigned int    old_width = r->szHint.width,
		    old_height = r->szHint.height;
    int		    fix_screen;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "%s(r, width=%u, height=%u)\n", __func__, width, height));
    /* update ncol/nrow of new window */
    fix_screen = rxvt_calc_colrow (r, width, height);

    /*
     * Check for refresh. If called XResizeWindow, and the WM politely refused
     * to resize window, then we should still process the resize event.
     */
    if (r->h->want_resize ||
	    old_width != r->szHint.width || old_height != r->szHint.height)
	rxvt_resize_sub_windows (r);

    if (fix_screen)
    {
	for (i = 0; i <= LTAB(r); i ++)
	{
	    int		curr_screen = -1;
	    uint16_t	old_ncol;

	    /*
	     * Update previous columns and rows for each VT. We need these
	     * copies so that rxvt_scr_reset can work for each VT. If all VT
	     * only share one copy, the first VT that calls rxvt_scr_reset will
	     * update it, thus other VT will not correctly adjust their
	     * column/row.
	     */
	    PVTS(r, i)->prev_nrow = r->h->prev_nrow;
	    PVTS(r, i)->prev_ncol = r->h->prev_ncol;
	    old_ncol = PVTS(r, i)->prev_ncol;

	    rxvt_scr_clear(r, i);
#ifdef BACKGROUND_IMAGE
	    rxvt_resize_pixmap(r, i);
#endif

	    curr_screen = rxvt_scr_change_screen(r, i, PRIMARY);
	    /* scr_reset only works on the primary screen */
	    rxvt_scr_reset(r, i);
	    if (curr_screen >= 0)
	    {
		/* this is not the first time through */
		rxvt_scr_change_screen(r, i, curr_screen);
		rxvt_selection_check(r, i, (old_ncol != r->TermWin.ncol ? 4 : 0));
	    }
	}
    }

#ifdef USE_XIM
    rxvt_IM_resize(r);
#endif
}