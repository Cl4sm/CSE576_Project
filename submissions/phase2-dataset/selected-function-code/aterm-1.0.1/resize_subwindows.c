Bool
resize_subwindows(int width, int height)
{
    int             x = 0, y = 0;
    int             old_width = TermWin.width;
    int             old_height = TermWin.height;

    TermWin.width = TermWin.ncol * TermWin.fwidth;
    TermWin.height = TermWin.nrow * TermWin.fheight;

/* size and placement */
    if (scrollbar_visible()) {
	scrollBar.beg = 0;
	scrollBar.end = height;
#ifndef XTERM_SCROLLBAR
# ifdef NEXT_SCROLLBAR
    /* arrows can be different */
	scrollBar.end -= GetScrollArrowsHeight();
# else
    /* arrows are as high as wide - leave 1 pixel gap */
	scrollBar.beg += (SB_WIDTH + 1) + sb_shadow;
	scrollBar.end -= (SB_WIDTH + 1) + sb_shadow;
# endif
#endif

	width -= (SB_WIDTH + 2 * sb_shadow);
	if (Options & Opt_scrollBar_right)
	    XMoveResizeWindow(Xdisplay, scrollBar.win, width, 0,
			      (SB_WIDTH + 2 * sb_shadow), height);
	else {
	    XMoveResizeWindow(Xdisplay, scrollBar.win, 0, 0,
			      (SB_WIDTH + 2 * sb_shadow), height);
	    x = (SB_WIDTH + 2 * sb_shadow);	/* placement of vt window */
	}
    }
    { /* ONLYIF MENUBAR */
	if (menubar_visible()) {
	    y = menuBar_TotalHeight();	/* for placement of vt window */
	    Resize_menuBar(x, 0, width, y);
	}
    }
    XMoveResizeWindow(Xdisplay, TermWin.vt, x, y, width, height + 1);
	
	request_background_update();

    if (old_width)
	Gr_Resize(old_width, old_height);

    XSync(Xdisplay, 0);
	return (width != old_width || height != old_height );
}
