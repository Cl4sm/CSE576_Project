void
rxvt_tabbar_highlight_tab (rxvt_t* r, short page, Bool force)
{
    register int    i, x;
    int		    sx, sy;
    unsigned int    rw, rh;
    XGCValues	    gcvalues;


    /* Sanatization */
    assert (LTAB(r) >= 0);
    assert (FVTAB(r) >= 0);
    assert (FVTAB(r) <= LTAB(r));
    assert (LVTAB(r) >= 0);
    assert (LVTAB(r) <= LTAB(r));
    assert (ATAB(r) >= FVTAB(r));
    assert (ATAB(r) <= LVTAB(r));

    assert (page <= LTAB(r));

    /* highlight flag is already set, simply return */
    if ( !force && PVTS(r, page)->highlight)
	return;	

    /* set highlight flag */
    PVTS(r, page)->highlight = 1;

    if (LTAB(r) < 0 || NOT_WIN(r->tabBar.win) || !r->tabBar.state)
	return ;

    /* do not highlight invisible/active tab */
    if (page < FVTAB(r) || page > LVTAB(r) || page == ATAB(r))
	return;

    for (i = FVTAB(r), x=TAB_BORDER; i < page; x += TAB_WIDTH(i), i++);

    /* set dash-line attributes */
    XGetGCValues( r->Xdisplay, r->tabBar.gc,
	    GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle,
	    &gcvalues);
    XSetLineAttributes (r->Xdisplay, r->tabBar.gc,
	    1, LineOnOffDash, CapButt, JoinMiter);

    XSetForeground (r->Xdisplay, r->tabBar.gc, r->tabBar.ifg);

    /* Set dimensions of the highlighted tab rectangle */
    sx = x + ( TXT_XOFF / 2 );
    sy = ISSET_OPTION(r, Opt2_bottomTabbar) ?
		TAB_TOPOFF + 1			:
		TAB_TOPOFF + ATAB_EXTRA + 1;
    rw = PVTS(r, page)->tab_width - TXT_XOFF;
    rh = TAB_BOTOFF - TAB_TOPOFF - ATAB_EXTRA - 3;

    XDrawRectangle (r->Xdisplay, r->tabBar.win, r->tabBar.gc,
	sx, sy, rw, rh);

    /* restore solid-line attributes */
    XChangeGC( r->Xdisplay, r->tabBar.gc,
	    GCLineWidth | GCLineStyle | GCCapStyle | GCJoinStyle,
	    &gcvalues);
}