void
rxvt_resize_on_subwin (rxvt_t* r, resize_reason_t reason)
{
    /*
     * Store the old width and height
     */
    unsigned oldWidth  = r->szHint.width;
    unsigned oldHeight = r->szHint.height;

    switch (reason)
    {
#ifdef HAVE_MENUBAR
	case HIDE_MENUBAR:
	    r->szHint.base_height -= rxvt_menubar_rheight (r);
	    r->szHint.min_height  -= rxvt_menubar_rheight (r);
	    r->szHint.height	  -= rxvt_menubar_rheight (r);
	    break;
	case SHOW_MENUBAR:
	    r->szHint.base_height += rxvt_menubar_rheight (r);
	    r->szHint.min_height  += rxvt_menubar_rheight (r);
	    r->szHint.height	  += rxvt_menubar_rheight (r);
	    break;
#endif	/* HAVE_MENUBAR */

	case HIDE_TABBAR:
	    r->szHint.base_height -= rxvt_tabbar_rheight (r);
	    r->szHint.min_height  -= rxvt_tabbar_rheight (r);
	    r->szHint.height	  -= rxvt_tabbar_rheight (r);
	    break;
	case SHOW_TABBAR:
	    r->szHint.base_height += rxvt_tabbar_rheight (r);
	    r->szHint.min_height  += rxvt_tabbar_rheight (r);
	    r->szHint.height	  += rxvt_tabbar_rheight (r);
	    break;

#ifdef HAVE_SCROLLBARS
	case HIDE_SCROLLBAR:
	    r->szHint.base_width -= rxvt_scrollbar_rwidth (r);
	    r->szHint.min_width  -= rxvt_scrollbar_rwidth (r);
	    r->szHint.width	 -= rxvt_scrollbar_rwidth (r);
	    break;
	case SHOW_SCROLLBAR:
	    r->szHint.base_width += rxvt_scrollbar_rwidth (r);
	    r->szHint.min_width  += rxvt_scrollbar_rwidth (r);
	    r->szHint.width	 += rxvt_scrollbar_rwidth (r);
	    break;
#endif	/* HAVE_SCROLLBARS */

	case RESIZE_FONT:
	    /* Calculate the base width and height */
	    r->szHint.base_width  = 2 * r->TermWin.int_bwidth;
	    r->szHint.base_height = 2 * r->TermWin.int_bwidth;
#ifdef HAVE_SCROLLBARS
	    if( rxvt_scrollbar_visible( r) )
		r->szHint.base_width += rxvt_scrollbar_width (r);
#endif
#ifdef HAVE_MENUBAR
	    if( rxvt_menubar_visible(r) )
		r->szHint.base_height += rxvt_menubar_height (r);
#endif
	    if( rxvt_tabbar_visible( r ) )
		r->szHint.base_height += rxvt_tabbar_height (r);

	    /* Set the terminal incremental width and height */
#ifndef NO_FRILLS
	    if(ISSET_OPTION(r, Opt2_smoothResize))
	    {
		r->szHint.width_inc = 1;
		r->szHint.height_inc = 1;
	    }
	    else
#endif
	    {
		r->szHint.width_inc = r->TermWin.fwidth;
		r->szHint.height_inc = r->TermWin.fheight;
	    }

	    /* Set the terminal minimal width and height */
	    r->szHint.min_width = r->szHint.base_width + r->TermWin.fwidth;
	    r->szHint.min_height = r->szHint.base_height + r->TermWin.fheight;

	    /* Calculate new height and width */
	    r->szHint.width  = r->szHint.base_width +
		    Width2Pixel(r->TermWin.ncol);
	    r->szHint.height = r->szHint.base_height +
		    Height2Pixel(r->TermWin.nrow);

	    /*
	     * Just incase the window is not resized by the WM, then we still need
	     * to refresh the screen.
	     */
	    r->h->want_resize |= FORCE_REFRESH;
	    break;

	default:
	    assert (0);	/* should not reach here */
	    return ;
    }

    /* Reset WMNormal Hints. We need not worry about r->szHint.flags */
    XSetWMNormalHints (r->Xdisplay, r->TermWin.parent, &(r->szHint));

    /* Set the terminal window starting position */
    if (NOTSET_OPTION(r, Opt_scrollBar_right))
	r->h->window_vt_x = ISSET_OPTION(r, Opt_scrollBar_right) ? 
	    0 : r->szHint.base_width - 2*r->TermWin.int_bwidth;

    r->h->window_vt_y = r->szHint.base_height - 2*r->TermWin.int_bwidth;
    if (ISSET_OPTION(r, Opt2_bottomTabbar))
	r->h->window_vt_y -= rxvt_tabbar_height (r);

    /*
     * Now we can resize the window The resize request might not always succeed.
     * If the window is already maximized, then (most?) WMs do nothing on the
     * below call. So when we recieve a ConfigureNotify event, we'll think we've
     * not moved or resized, and will erroriously not update our TermWin.ncols
     * etc.
     */
    XResizeWindow(r->Xdisplay, r->TermWin.parent,
	    r->szHint.width, r->szHint.height);

    /*
     * We get around this error by setting want_resize. In configure notify
     * events, if this variable is set, we resize the window regardless of
     * wether it apears to be in the same position or not.
     */
    r->h->want_resize |= FORCE_RESIZE;

#ifndef NO_FRILLS
    while(ISSET_OPTION(r, Opt2_smartResize))
    {
	/*
	 * Let's attempt to move the window so we don't push part of it off
	 * screen.
	 */

	CARD32	 left, right, top, bottom;
	int	 wx, wy;	  /* pos of window with decorations */
	unsigned wwidth, wheight; /* dim of window with decorations */

	int newx = r->szHint.x,   /* new location to move to */
	    newy = r->szHint.y;

	unsigned    dspw = DisplayWidth(  r->Xdisplay, XSCREEN),
		    dsph = DisplayHeight( r->Xdisplay, XSCREEN);

	int  dx = (int) r->szHint.width - (int) oldWidth,
	     dy = (int) r->szHint.height- (int) oldHeight;


	if( !getWMStruts( r->Xdisplay, r->TermWin.parent,
		&left, &right, &top, &bottom)) break;

	wx = r->szHint.x - left;
	wy = r->szHint.y - top;
	wwidth  = r->szHint.width  + left + right;
	wheight = r->szHint.height + top + bottom;

	/*
	 * Now the position of the window with decorations is in wx, wy,
	 * wwidth, wheight.
	 */
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Strut: (%lu, %lu %lu, %lu), " "Geometry: %ux%u+%d+%d, Change: (%d, %d)\n", left, right, top, bottom, wwidth, wheight, wx, wy, dx, dy ));

	/*
	 * Try and move us back on screen, if we resized off it.
	 */
	if( wwidth >= dspw || wheight >= dsph ) break;

	if( wx < 0 && wx + dx >= 0) newx = left;
	else if( wx + (int) wwidth > (int) dspw
		&& wx + (int) wwidth - dx <= (int) dspw )
	    newx = dspw - wwidth + left;

	if( wy < 0 && wy + dy >= 0) newy = top;
	else if( wy + (int) wheight > (int) dsph
		&& wy + (int) wheight - dy <= (int) dsph)
	    newy = dsph - wheight + top;

	if(
		(newx != r->szHint.x || newy != r->szHint.y) /* moved */
		&& newx - (int) left >=0	/* onscreen */
		&& newx - (int) left + (int) wwidth <= dspw
		&& newy - (int) top >=0
		&& newy - (int) top + (int) wheight <= dsph
	  )
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Smart move to %ux%u+%d+%d\n", r->szHint.width, r->szHint.height, newx, newy));
	    XMoveWindow( r->Xdisplay, r->TermWin.parent,
		    newx, newy);
	}
	break;
    }
#endif

    /* 
     * According to X Reference, we should only trust the size of a Window
     * through ConfigureNotify event, or through XGet... functions. So here we
     * must not change the window size till we receive a ConfigureNotify event.
     */
    r->szHint.width = oldWidth;
    r->szHint.height = oldHeight;
}