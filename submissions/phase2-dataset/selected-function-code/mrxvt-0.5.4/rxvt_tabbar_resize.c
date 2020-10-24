void
rxvt_tabbar_resize (rxvt_t* r)
{
    register int    i;
    int		    sx, sy;


    sx = 0;
    sy = 0;
#ifdef HAVE_MENUBAR
    sy += rxvt_menubar_height (r);
#endif
    if (ISSET_OPTION(r, Opt2_bottomTabbar))
	sy += VT_HEIGHT(r);
    XMoveResizeWindow  (r->Xdisplay, r->tabBar.win,
	sx, sy, TWIN_WIDTH(r), rxvt_tabbar_rheight (r));

    /* recompute width of each tab */
    for (i = 0; i <= LTAB(r); i ++)
	PVTS(r, i)->tab_width = rxvt_tab_width (r, PVTS(r, i)->tab_title);

    /* adjust visible tabs */
    rxvt_tabbar_set_visible_tabs (r, False);
    /* redraw the tabs and buttons */
    XClearArea( r->Xdisplay, r->tabBar.win,
	    0, 0, 0, 0, True);
}