void
rxvt_tabbar_set_title (rxvt_t* r, short page, const unsigned char TAINTED * str)
{
    char UNTAINTED *	    n_title;

    assert (str);
    assert (page >= 0 && page <= LTAB(r));
    assert (PVTS(r, page)->tab_title);

    n_title = STRNDUP (str, MAX_TAB_TXT);
    /*
     * If strdup succeeds, set new title
     */
    if (NULL != n_title)
    {
	rxvt_free (PVTS(r, page)->tab_title);
	PVTS(r, page)->tab_title = n_title;

	/* Compute the new width of the tab */
	PVTS(r, page)->tab_width = rxvt_tab_width (r, n_title);
    }

    /*
     * If visible tab's title is changed, refresh tab bar
     */
    if (page >= FVTAB(r) && page <= LVTAB(r))
    {
	/* adjust visible tabs */
	rxvt_tabbar_set_visible_tabs (r, True);
	refresh_tabbar_tab(r, page);
    }

    /* synchronize terminal title with active tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle) &&
	(page == ATAB(r)))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon) &&
	(page == ATAB(r)))
	rxvt_set_icon_name(r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}