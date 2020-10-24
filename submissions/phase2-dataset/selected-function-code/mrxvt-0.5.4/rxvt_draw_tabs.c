void rxvt_draw_tabs (rxvt_t* r, Region region)
{
    int	    page, x;

    if (LTAB(r) < 0 || NOT_WIN(r->tabBar.win) || !r->tabBar.state)
	/*
	 * Nothing to do here :)
	 */
	return;

    /* Sanatization */
    assert( LTAB(r)  >= 0	 );
    assert( FVTAB(r) >= 0	 );
    assert( FVTAB(r) <= LTAB(r)	 );
    assert( LVTAB(r) >= 0	 );
    assert( LVTAB(r) <= LTAB(r)	 );
    assert( ATAB(r)  >= FVTAB(r) );
    assert( ATAB(r)  <= LVTAB(r) );


    if (IS_REGION(region))
	XSetRegion( r->Xdisplay, r->tabBar.gc, region);

    for( page=FVTAB(r), x=TAB_BORDER; page <= LVTAB(r); page++)
    {
	/*
	 * Draw the tab corresponding to "page".
	 */
	XArc	 	arcs[2];
	XPoint	 	points[8];

	/*
	 * Color the tab as active if the tab is active 
	 * or
	 * activity/inactivity is detected by the MonitorTab macro
	 */

	if( (page == ATAB(r)) || 
 	    ((NOT_NULL(&PVTS(r, page)->monitor_tab)) && 
	     (PVTS(r,page)->monitor_tab == TAB_MON_NOTIFICATION)) 
	  )
	{
	    /* 
	     * disable activity/inactivity notification if current tab is
	     * active tab
	     */
	    if ((page == ATAB(r)) && 
		(PVTS(r,page)->monitor_tab == TAB_MON_NOTIFICATION))
	    {
	      rxvt_msg (DBG_INFO, DBG_MACROS,  
		      "Macro MonitorTab: monitored tab %i is now the active tab", page);
	      PVTS(r,page)->monitor_tab = TAB_MON_OFF;
            }
	    /*
	     * Draw the active tab, and bottom line of the tabbar.
	     */

	    int		    clear = 0;	/* use ClearArea or FillRectangle */

	    if (ISSET_OPTION(r, Opt2_bottomTabbar))
	    {
		/* Top tabbar line & left of active tab */
		SET_POINT( points[0], 0, TAB_TOPOFF);
		SET_POINT( points[1], x, TAB_TOPOFF);
		SET_POINT( points[2], x, TAB_BOTOFF - TAB_RADIUS);

		/* Arc coordinates for rounded tab tops :) */
		SET_ARC( arcs[0], x, TAB_BOTOFF - 2*TAB_RADIUS,
			2*TAB_RADIUS, 2*TAB_RADIUS, 180*64, 90*64);
		SET_ARC( arcs[1],
			x + AVTS(r)->tab_width - 2*TAB_RADIUS,
			TAB_BOTOFF - 2*TAB_RADIUS,
			2*TAB_RADIUS, 2*TAB_RADIUS, 270*64, 90*64);

		/* Coordinates for horizontal line below tab. */
		SET_POINT( points[3], x + TAB_RADIUS, TAB_BOTOFF);
		SET_POINT( points[4],
			x + AVTS(r)->tab_width - TAB_RADIUS, TAB_BOTOFF);

		/* Right line of tab and top of tabbar. */
		SET_POINT( points[5],
			x + AVTS(r)->tab_width, TAB_BOTOFF - TAB_RADIUS);
		SET_POINT( points[6], x + AVTS(r)->tab_width, TAB_TOPOFF);
		SET_POINT( points[7], TWIN_WIDTH(r), TAB_TOPOFF);
	    }

	    else    /* if (ISSET_OPTION(r, Opt2_bottomTabbar)) */
	    {
		/*
		 * Coordinates for the draw bottom line to the left of active
		 * tab, and left verticle line of the active tab.
		 */
		SET_POINT( points[0], 0, TAB_BOTOFF);
		SET_POINT( points[1], x, TAB_BOTOFF);
		SET_POINT( points[2], x, TAB_TOPOFF + TAB_RADIUS);

		/* Arc coordinates for rounded tab tops :) */
		SET_ARC( arcs[0], x, TAB_TOPOFF,
			2*TAB_RADIUS, 2*TAB_RADIUS, 180*64, -90*64);
		SET_ARC( arcs[1],
			x + AVTS(r)->tab_width - 2*TAB_RADIUS, TAB_TOPOFF,
			2*TAB_RADIUS, 2*TAB_RADIUS, 90*64, -90*64);

		/* Coordinates for horizontal line above tab. */
		SET_POINT( points[3], x + TAB_RADIUS, TAB_TOPOFF);
		SET_POINT( points[4],
			x + AVTS(r)->tab_width - TAB_RADIUS, TAB_TOPOFF);

		/*
		 * Coordinates for vertical line on the right of the active tab, and
		 * bottom line of tab bar after active tab.
		 */
		SET_POINT( points[5], x + AVTS(r)->tab_width,
			TAB_TOPOFF + TAB_RADIUS);
		SET_POINT( points[6], x + AVTS(r)->tab_width, TAB_BOTOFF);
		SET_POINT( points[7], TWIN_WIDTH(r), TAB_BOTOFF);
	    }

#ifdef BACKGROUND_IMAGE
	    if( r->tabBar.hasPixmap  && ISSET_OPTION(r, Opt_tabPixmap))
		clear = 1;  /* use background image */
#endif
#ifdef TRANSPARENT
	    if ( ( r->h->am_transparent || r->h->am_pixmap_trans ) &&
		ISSET_OPTION(r, Opt_transparent_tabbar))
		clear = 1;  /* transparent override background image */
#endif

	    if( !clear )
	    {
		/*
		 * Fill the ATAB with the background color.
		 */
		CHOOSE_GC_FG( r, r->tabBar.bg);

		XFillArcs( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			arcs, 2);
		XFillPolygon( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			points+1, 6, Convex, CoordModeOrigin);

		/*
		 * This misses the bottom of the ATAB, so we should color it
		 * ourselves.
		 *
		 * 2006-02-14 gi1242: Drawing with XDrawLine is not enough. For
		 * some reason a thin line below is still missed. Be super safe
		 * and XFillRectangle it.
		 *
		 * 2006-05-26 gi1242: The thin line looks kinda nice actually...
		 */
#if 0
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			points[1].x, points[1].y,
			points[6].x, points[6].y);

		XFillRectangle( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			points[1].x, points[1].y,
			points[6].x - points[1].x, 2);
#endif
	    }


	    /*
	     * Finally, draw the (boundary) of ATAB here.
	     */
	    CHOOSE_GC_FG( r, r->tabBar.frame);

	    /* Tabbar line + left of ATAB */
	    XDrawLines( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
		    points, 3, CoordModeOrigin);
	    /* Rounded tab tops :) */
	    XDrawArcs( r->Xdisplay, r->tabBar.win, r->tabBar.gc, arcs, 2);
	    /* Top line of ATAB */
	    XDrawLines( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
		    points + 3, 2, CoordModeOrigin);
	    /* Right of ATAB + tab bar line */
	    XDrawLines( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
		    points + 5, 3, CoordModeOrigin);

	    /* Draw the tab title. */
	    CHOOSE_GC_FG( r, r->tabBar.fg);
	    draw_title (r, x + TXT_XOFF, ATAB_EXTRA / 2 + TXT_YOFF,
		    page, region);
	}

	else /* if( page == ATAB(r) ) */
	{
	    /*
	     * Draw the inactive tabs.
	     */
	    CHOOSE_GC_FG( r, r->tabBar.delimit);

	    if (ISSET_OPTION(r, Opt2_bottomTabbar))
	    {
		/* Left vertical line */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x, TAB_TOPOFF + 1, /* Dont' interupt tabbar line */
			x, TAB_BOTOFF - TAB_RADIUS - ATAB_EXTRA);

		/* Draw rounded tab bottoms :). */
		SET_ARC( arcs[0], x, TAB_BOTOFF - ATAB_EXTRA - 2*TAB_RADIUS,
			2*TAB_RADIUS, 2*TAB_RADIUS, 180*64, 90*64);
		SET_ARC( arcs[1],
			x + PVTS(r, page)->tab_width - 2*TAB_RADIUS,
			TAB_BOTOFF - ATAB_EXTRA - 2*TAB_RADIUS,
			2*TAB_RADIUS, 2*TAB_RADIUS, 270*64, 90*64);
		XDrawArcs( r->Xdisplay, r->tabBar.win, r->tabBar.gc, arcs, 2);

		/* Horizontal line below tab. */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x + TAB_RADIUS, TAB_BOTOFF - ATAB_EXTRA,
			x + PVTS(r, page)->tab_width - TAB_RADIUS,
			TAB_BOTOFF - ATAB_EXTRA);

		/* Right vertical line */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x + PVTS(r, page)->tab_width,
			TAB_BOTOFF - TAB_RADIUS - ATAB_EXTRA,
			x + PVTS(r, page)->tab_width, TAB_TOPOFF + 1);
	    }

	    else /* if (ISSET_OPTION(r, Opt2_bottomTabbar)) */
	    {
		/* Left vertical line */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x, TAB_BOTOFF-1, x,
			TAB_TOPOFF + TAB_RADIUS + ATAB_EXTRA);

		/* Draw rounded tab tops :). */
		SET_ARC( arcs[0], x, TAB_TOPOFF + ATAB_EXTRA,
			2*TAB_RADIUS, 2*TAB_RADIUS, 180*64, -90*64);
		SET_ARC( arcs[1],
			x + PVTS(r, page)->tab_width - 2*TAB_RADIUS,
			TAB_TOPOFF + ATAB_EXTRA,
			2*TAB_RADIUS, 2*TAB_RADIUS, 90*64, -90*64);
		XDrawArcs( r->Xdisplay, r->tabBar.win, r->tabBar.gc, arcs, 2);

		/* Horizontal line above tab. */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x + TAB_RADIUS, TAB_TOPOFF + ATAB_EXTRA,
			x + PVTS(r, page)->tab_width - TAB_RADIUS,
			TAB_TOPOFF + ATAB_EXTRA);

		/* Right vertical line */
		XDrawLine( r->Xdisplay, r->tabBar.win, r->tabBar.gc,
			x + PVTS(r, page)->tab_width,
			TAB_TOPOFF + TAB_RADIUS + ATAB_EXTRA,
			x + PVTS(r, page)->tab_width, TAB_BOTOFF-1);
	    }

	    /* Choose GC foreground for tab title. */
	    CHOOSE_GC_FG( r, r->tabBar.ifg);
	    draw_title (r, x + TXT_XOFF,
		    ISSET_OPTION(r, Opt2_bottomTabbar) ?
		     	    TXT_YOFF : ATAB_EXTRA + TXT_YOFF,
		    page, region);

	    /* Highlight the tab if necessary */
	    if( PVTS(r, page)->highlight )
		rxvt_tabbar_highlight_tab( r, page, True);
	}


	x += TAB_WIDTH(page);
    }

    if (IS_REGION(region))
	XSetClipMask( r->Xdisplay, r->tabBar.gc, None);
}