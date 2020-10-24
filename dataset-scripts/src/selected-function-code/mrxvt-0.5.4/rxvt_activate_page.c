void
rxvt_activate_page (rxvt_t* r, short index)
{
    /* shortcut */
    if (/* !r->tabBar.state ||
	NOT_WIN(r->tabBar.win) || */
	index == ATAB(r))
	return;

    AVTS(r)->mapped = 0;
    r->tabBar.ptab = ATAB(r);
    ATAB(r) = index;
    AVTS(r)->mapped = 1;
    AVTS(r)->highlight = 0; /* clear highlight flag */
    
    /*
     * Now the visible tabs may be changed, recompute the visible
     * tabs before redrawing.
     */
    if (index < FVTAB(r) || index > LVTAB(r))
    {
	/* adjust visible tabs */
	rxvt_tabbar_set_visible_tabs (r, True);
    }
    refresh_tabbar_tab( r, ATAB(r));
    refresh_tabbar_tab( r, PTAB(r));

    /* Switch VT fg/bg colors */
    rxvt_set_vt_colors( r, ATAB(r) );
    XMapRaised( r->Xdisplay, AVTS(r)->vt );
    /*
     * We don't need to touch the screen here. XMapRaised will generate a
     * MapNotify and Expose events, which will refresh the screen as needed.
     * Touching the screen unnecessarily causes a flicker (and is *horrible*
     * under slow connections).
     */
    /* rxvt_scr_touch (r, ATAB(r), True); */
    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR, "active page is %d\n",ATAB(r)));

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name (r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}