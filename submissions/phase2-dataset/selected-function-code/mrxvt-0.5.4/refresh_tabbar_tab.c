void
refresh_tabbar_tab( rxvt_t *r, int page)
{
    int		i;
    XRectangle	rect;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "Refreshing tabbar title of page %d\n", page));

    if( page < FVTAB(r) || page > LVTAB(r) ) return;

    for( i=FVTAB(r), rect.x=TAB_BORDER; i < page; i++)
	rect.x += TAB_WIDTH(i);
    
    rect.y	= TAB_TOPOFF;
    rect.width	= TAB_WIDTH( page);
    rect.height	= 0;

    /* Clear the tab completely, and send expose events */
    XClearArea( r->Xdisplay, r->tabBar.win,
	    rect.x, rect.y, rect.width, rect.height, True);
}