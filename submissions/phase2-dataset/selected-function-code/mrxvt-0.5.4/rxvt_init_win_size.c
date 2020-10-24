void
rxvt_init_win_size( rxvt_t *r )
{
    int		    flags = 0;	/* must initialize to 0!!! */
    short	    recalc_x = 0, recalc_y = 0,
		    recalc_width = 1, recalc_height = 1;
    int		    x, y;
    unsigned int    w, h;


    r->szHint.flags = PMinSize | PResizeInc | PBaseSize | PWinGravity;
    r->szHint.win_gravity = NorthWestGravity;

    /* Set default terminal columns and rows */
    r->TermWin.ncol = 80;
    r->TermWin.nrow = 24;
    r->szHint.x = 0;
    r->szHint.y = 0;

#ifdef TRANSPARENT
    refreshRootBGVars( r );
#endif

    /* Get geometry in x, y, w, h */
    if (r->h->rs[Rs_geometry])
	flags = XParseGeometry(r->h->rs[Rs_geometry], &x, &y, &w, &h);

    /* Calculate the terminal increment width and height */
#ifndef NO_FRILLS
    if( ISSET_OPTION(r, Opt2_smoothResize))
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

    /* Calculate the base width and height */
    r->szHint.base_width = 2 * r->TermWin.int_bwidth;
    r->szHint.base_height = 2 * r->TermWin.int_bwidth;
#ifdef HAVE_SCROLLBARS
    if (ISSET_OPTION(r, Opt_scrollBar))
	r->szHint.base_width += rxvt_scrollbar_rwidth (r);
#endif
#ifdef HAVE_MENUBAR
    if (ISSET_OPTION(r, Opt_showMenu))
	r->szHint.base_height += rxvt_menubar_rheight (r);
#endif
    if (NOTSET_OPTION(r, Opt2_hideTabbar))
	r->szHint.base_height += rxvt_tabbar_rheight (r);

    /* Set the terminal minimal width and height */
    r->szHint.min_width = r->szHint.base_width + r->TermWin.fwidth;
    r->szHint.min_height = r->szHint.base_height + r->TermWin.fheight;

    /* Parse Geometry */
    if (flags & WidthValue)
    {
	r->TermWin.ncol = BOUND_POSITIVE_INT16(w);
#ifndef NO_FRILLS
	if( ISSET_OPTION(r, Opt2_smoothResize) )
	{
	    /* For smoothResize, w as a pixel width (if large enough) */
	    if(r->TermWin.ncol > r->szHint.base_width + r->TermWin.fwidth)
	    {
		r->szHint.width = r->TermWin.ncol;
		r->TermWin.ncol = (r->TermWin.ncol - r->szHint.base_width) /
							r->TermWin.fwidth;
		recalc_width = 0;
	    }
	    else r->TermWin.ncol = 1;
	}
#endif
	r->szHint.flags |= USSize;
    }
    if (flags & HeightValue)
    {
	r->TermWin.nrow = BOUND_POSITIVE_INT16(h);
#ifndef NO_FRILLS
	if(ISSET_OPTION(r, Opt2_smoothResize))
	{
	    /* For smoothResize, w as a pixel height (if large enough) */
	    if(r->TermWin.nrow > r->szHint.base_height + r->TermWin.fheight)
	    {
		r->szHint.height = r->TermWin.nrow;
		r->TermWin.nrow =
		    (r->TermWin.nrow - r->szHint.base_height) / r->TermWin.fheight;
		recalc_height = 0;
	    }
	    else r->TermWin.nrow = 1;
	}
#endif
	r->szHint.flags |= USSize;
    }
    if (flags & XValue)
    {
	r->szHint.x = x;
	r->szHint.flags |= USPosition;
	if (flags & XNegative)
	{
	    recalc_x = 1;
	    r->szHint.win_gravity = NorthEastGravity;
	}
    }
    if (flags & YValue)
    {
	r->szHint.y = y;
	r->szHint.flags |= USPosition;
	if (flags & YNegative)
	{
	    recalc_y = 1;
	    if (r->szHint.win_gravity == NorthEastGravity)
		r->szHint.win_gravity = SouthEastGravity;
	    else
		r->szHint.win_gravity = SouthWestGravity;
	}
    }

    /* Set the terminal width and height */
    if( recalc_width)
	r->szHint.width = r->szHint.base_width + Width2Pixel (r->TermWin.ncol);
    if( recalc_height)
	r->szHint.height = r->szHint.base_height + Height2Pixel (r->TermWin.nrow);

    /* Recalculate the starting position */
    if (recalc_x)
	r->szHint.x += (DisplayWidth(r->Xdisplay, XSCREEN)
	    - r->szHint.width - 2 * r->TermWin.ext_bwidth);
    if (recalc_y)
	r->szHint.y += (DisplayHeight(r->Xdisplay, XSCREEN)
	    - r->szHint.height - 2 * r->TermWin.ext_bwidth);

    /* Set the terminal window starting position */
    r->h->window_vt_x = (ISSET_OPTION(r, Opt_scrollBar_right)) ? 
	    0 : r->szHint.base_width - 2*r->TermWin.int_bwidth;
    r->h->window_vt_y = r->szHint.base_height - 2*r->TermWin.int_bwidth;
    if (ISSET_OPTION(r, Opt2_bottomTabbar) && NOTSET_OPTION(r, Opt2_hideTabbar))
	r->h->window_vt_y -= rxvt_tabbar_rheight (r);
}