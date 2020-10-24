void
rxvt_tabbar_draw_buttons (rxvt_t* r)
{
    register int    i;
    int		    topoff;
    unsigned long   frame;


    if (LTAB(r) < 0)
	return;
    if (NOT_WIN(r->tabBar.win))
	return;
    if (!r->tabBar.state)
	return;

    /* whether the buttons are hidden */
    if (ISSET_OPTION(r, Opt2_hideButtons))
	return;

    topoff = BTN_TOPOFF;
#if 0
    frame = NOTSET_OPTION(r, Opt2_bottomTabbar) ?
		r->tabBar.frame : r->tabBar.delimit;
#endif
    frame = r->tabBar.frame;

    CHOOSE_GC_FG (r, r->tabBar.fg);
    for (i = NB_XPM; i >= 1; i--)
    {
#ifdef HAVE_LIBXPM
	register int	curimg = NB_XPM - i;

	switch (curimg)
	{
	    case XPM_TERM:
		img[XPM_TERM] = (LTAB(r) == MAX_PAGES - 1) ? 
		    img_d[XPM_TERM] : img_e[XPM_TERM];
		break;
	    case XPM_CLOSE:
		img[XPM_CLOSE] = (ISSET_OPTION(r, Opt2_protectSecondary) &&
				PRIMARY != AVTS(r)->current_screen) ?
			img_d[XPM_CLOSE] : img_e[XPM_CLOSE];
		break;
	    case XPM_LEFT:
		img[XPM_LEFT] = (FVTAB(r) == 0) ? 
		    img_d[XPM_LEFT] : img_e[XPM_LEFT];
		break;
	    case XPM_RIGHT:
		img[XPM_RIGHT] = (LVTAB(r) == LTAB(r)) ? 
		    img_d[XPM_RIGHT] : img_e[XPM_RIGHT];
		break;
	}
#endif
	if (IS_PIXMAP(img[NB_XPM-i]))
	{
	    XCopyArea  (r->Xdisplay, img[NB_XPM-i], r->tabBar.win,
		r->tabBar.gc, 0, 0,
		BTN_WIDTH, BTN_HEIGHT,
		TWIN_WIDTH(r)-(i*(BTN_WIDTH+BTN_SPACE)), topoff);
	}
    }


    CHOOSE_GC_FG (r, r->tabBar.frame);
    for (i = NB_XPM; i >= 1; i--)
    {
	/*
	XDrawRectangle (r->Xdisplay, r->tabBar.win,
	    r->tabBar.gc,
	    TWIN_WIDTH(r)-(i*(BTN_WIDTH+BTN_SPACE)), topoff,
	    BTN_WIDTH, BTN_HEIGHT);
	*/
	int	sx = TWIN_WIDTH(r) - (i*(BTN_WIDTH+BTN_SPACE));
	/* draw top line */
	XDrawLine (r->Xdisplay, r->tabBar.win, r->tabBar.gc,
	    sx, topoff, sx + BTN_WIDTH, topoff);
	/* draw left line */
	XDrawLine (r->Xdisplay, r->tabBar.win, r->tabBar.gc,
	    sx, topoff, sx, topoff + BTN_HEIGHT);
    }
    CHOOSE_GC_FG (r, r->tabBar.delimit);
    for (i = NB_XPM; i >= 1; i--)
    {
	int	sx = TWIN_WIDTH(r) - (i*(BTN_WIDTH+BTN_SPACE));
	/* draw bottom line */
	XDrawLine (r->Xdisplay, r->tabBar.win, r->tabBar.gc,
	    sx, topoff+BTN_HEIGHT, sx+BTN_WIDTH, topoff+BTN_HEIGHT);
	/* draw right line */
	XDrawLine (r->Xdisplay, r->tabBar.win, r->tabBar.gc,
	    sx+BTN_WIDTH, topoff, sx+BTN_WIDTH, topoff+BTN_HEIGHT);
    }
}