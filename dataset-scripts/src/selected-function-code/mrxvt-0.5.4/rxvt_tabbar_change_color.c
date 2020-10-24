void
rxvt_tabbar_change_color (rxvt_t* r, int item, const char* str)
{
    XColor	xcol;
    int		changed = 0;


    switch (item)
    {
	case MRxvt_tabfg:
	    if (r->h->rs[Rs_tabfg] &&
		!STRCASECMP(str, r->h->rs[Rs_tabfg]))
		break;	/* no color change */

	    if (rxvt_parse_alloc_color (r, &xcol, str))
	    {
		r->tabBar.fg = xcol.pixel;
#ifdef XFT_SUPPORT
		rxvt_alloc_xft_color( r, &xcol, &(r->tabBar.xftfg) );
#endif
		if (r->tabBar.rsfg)	/* free previous string */
		    rxvt_free ((void*) r->h->rs[Rs_tabfg]);
		r->h->rs[Rs_tabfg] = STRDUP(str);
		r->tabBar.rsfg = 1;	/* free resource string later */
		changed = 1;
	    }
	    break;

	case MRxvt_tabbg:
	    if (r->h->rs[Rs_tabbg] &&
		!STRCASECMP(str, r->h->rs[Rs_tabbg]))
		break;	/* no color change */

	    if (rxvt_parse_alloc_color (r, &xcol, str))
	    {
		r->tabBar.bg = xcol.pixel;
		if (r->tabBar.rsbg)	/* free previous string */
		    rxvt_free ((void*) r->h->rs[Rs_tabbg]);
		r->h->rs[Rs_tabbg] = STRDUP(str);
		r->tabBar.rsbg = 1;	/* free resource string later */
		changed = 1;
	    }
	    break;

	case MRxvt_itabfg:
	    if (r->h->rs[Rs_itabfg] &&
		!STRCASECMP(str, r->h->rs[Rs_itabfg]))
		break;	/* no color change */

	    if (rxvt_parse_alloc_color (r, &xcol, str))
	    {
		r->tabBar.ifg = xcol.pixel;
#ifdef XFT_SUPPORT
		rxvt_alloc_xft_color( r, &xcol, &(r->tabBar.xftifg) );
#endif
		if (r->tabBar.rsifg)	/* free previous string */
		    rxvt_free ((void*) r->h->rs[Rs_itabfg]);
		r->h->rs[Rs_itabfg] = STRDUP(str);
		r->tabBar.rsifg = 1;	/* free resource string later */
		changed = 1;
	    }
	    break;

	case MRxvt_itabbg:
	    if (r->h->rs[Rs_itabbg] && !STRCASECMP(str, r->h->rs[Rs_itabbg]))
		break;

	    if (rxvt_parse_alloc_color (r, &xcol, str))
	    {
		r->tabBar.ibg = xcol.pixel;
		if (r->tabBar.rsibg)	/* free previous string */
		    rxvt_free ((void*) r->h->rs[Rs_itabbg]);
		r->h->rs[Rs_itabbg] = STRDUP(str);
		r->tabBar.rsibg = 1;	/* free resource string later */
		changed = 1;
	    }
	    break;
	
	default:
	    break;
    }

    if (changed)
    {
	if (MRxvt_itabbg == item)
	{
#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
	    if (
# ifdef TRANSPARENT
		    (
		     (r->h->am_transparent || r->h->am_pixmap_trans)
		     && ISSET_OPTION (r, Opt_transparent_tabbar)
		    )
# endif
# if defined(TRANSPARENT) && defined(BACKGROUND_IMAGE)
		||
# endif
# ifdef BACKGROUND_IMAGE
		( r->tabBar.hasPixmap )
# endif
	       )
	    {
# ifdef HAVE_LIBXRENDER
		/* Background image needs to be regrabed */
		rxvt_refresh_bg_image(r, ATAB(r), False);
# endif
	    }
	    else
#endif
	    {
		XSetWindowBackground (r->Xdisplay, r->tabBar.win,
			r->tabBar.ibg);
	    }
	}

	/*
	 * Better to put the expose event on the queue, than expose immediately.
	 * Expose events can be expensive when using XRender transparency.
	 */
	XClearArea( r->Xdisplay, r->tabBar.win, 0, 0, 0, 0, True);
    }
}