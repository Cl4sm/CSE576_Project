void
rxvt_tabbar_set_visible_tabs (rxvt_t* r, Bool refresh)
{
    assert( LTAB(r) >= 0 );

    /*
     * For Firefox style tabs, we should recompute all tabwidths.
     */
#ifdef XFT_SUPPORT
    if( ISSET_OPTION(r, Opt_xft) && r->TermWin.xftpfont )
    {
	int	i;
	short	tabWidth = rxvt_tab_width( r, NULL);	/* Firefox style tabs
							   don't need the tab
							   title */
	int	numVisible = (TAB_SPACE - TAB_BORDER)
				/ (TAB_BORDER + tabWidth);

	int	oldTabWidth = PVTS(r,0)->tab_width,
		oldFVtab    = FVTAB(r),
		oldLVtab    = LVTAB(r);

	/*
	 * Reset the widths of all tabs (visible or not).
	 */
	for (i = 0; i <= LTAB(r); i ++) PVTS(r, i)->tab_width = tabWidth;

	/*
	 * Set visible tabs. First make sure the active tab is visible
	 */
	if( numVisible == 1 )
	    FVTAB(r) = LVTAB(r) = ATAB(r);
	else
	{
	    if( ATAB(r) < FVTAB(r) )
		/* Make ATAB second last tab that's visible */
		FVTAB(r) = max( ATAB(r) - numVisible + 2, 0);
	    else if ( ATAB(r) >= FVTAB(r) + numVisible )
		/* Make ATAB the second tab that's visible */
		FVTAB(r) = max( ATAB(r) - 1, 0);

	    /*
	     * Active tab is now visible. Try and make as many other tabs
	     * visible.
	     */
	    if( FVTAB(r) + numVisible - 1 > LTAB(r) )
	    {
		LVTAB(r) = LTAB(r);
		FVTAB(r) = max( LVTAB(r) - numVisible + 1, 0);
	    }
	    else
		LVTAB(r) = FVTAB(r) + numVisible - 1;
	}

	if( refresh && IS_WIN(r->tabBar.win))
	{
	    /* Clear out the parts of the tabbar that have changed. Expose
	     * events will be sent to the tabbar. */
	    if( tabWidth != oldTabWidth || FVTAB(r) != oldFVtab )
		/* Refresh all tabs */
		XClearArea( r->Xdisplay, r->tabBar.win,
			0, 0, TAB_SPACE, 0, True);

	    else if( oldLVtab != LVTAB(r) )
	    {
		int x = TAB_BORDER +
		    (TAB_BORDER + tabWidth) * min( oldLVtab, LVTAB(r));
		XClearArea( r->Xdisplay, r->tabBar.win,
			x, 0, TAB_SPACE - x + 1, 0, True);
	    }
	}
    }
    else
#endif
    {
	/* set first visible tab to active tab */
	FVTAB(r) = ATAB(r);

	/* always try visualize the right tabs */
	LVTAB(r) = find_right_tab (r, FVTAB(r),
	    TAB_SPACE - TAB_WIDTH(FVTAB(r)));

	if (LVTAB(r) == LTAB(r) && 0 != FVTAB(r))
	{
	    /* now try to visualize the left tabs */
	    register int size = TAB_SPACE -
		width_between (r, FVTAB(r), LVTAB(r));

	    FVTAB(r) = find_left_tab (r, FVTAB(r), size);
	}
	if( refresh && IS_WIN(r->tabBar.win))
	    XClearArea( r->Xdisplay, r->tabBar.win, 0, 0, TAB_SPACE, 0, True);
    }
}