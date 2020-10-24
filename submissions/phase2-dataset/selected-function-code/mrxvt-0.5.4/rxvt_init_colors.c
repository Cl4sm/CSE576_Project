void
rxvt_init_colors( rxvt_t *r )
{
    register int    i;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_INIT, "%s()\n", __func__));

    /* Initialize fg/bg colors for each profile */
    for (i = 0; i < MAX_PROFILES; i++)
    {
	XColor	    xcol;
	int	    vtfg = Rs_foreground + i;
	int	    vtbg = Rs_background + i;

	if( !ISSET_VTFG( r, i ) )
	    r->h->rs[vtfg] =  ISSET_VTFG( r, 0 ) ?
		    r->h->rs[Rs_foreground] : def_colorName[ Color_fg ];
	if( !ISSET_VTBG( r, i ) )
	    r->h->rs[vtbg] = ISSET_VTBG( r, 0 ) ?
		    r->h->rs[Rs_background] : def_colorName[ Color_bg ];

	/* foreground color of i terminal */
	if( rxvt_parse_alloc_color(r, &xcol, r->h->rs[vtfg]) )
	{
	    VTFG(r, i) = xcol.pixel;

#ifdef XFT_SUPPORT
	    rxvt_alloc_xft_color( r, &xcol, &(VTXFTFG(r, i)) );
	    rxvt_fade_color( r, &xcol, &VTFG_FADE(r, i), &VTXFTFG_FADE(r, i) );
#else
	    rxvt_fade_color( r, &xcol, &VTFG_FADE(r, i), NULL );
#endif /* XFT_SUPPORT */
	}

	else
	{
	    rxvt_msg (DBG_ERROR, DBG_INIT,  "Could not alloc foreground color of profile %d",
		    i );
	    if( i == 0 )
		/* Need default fg/bg */
		exit( EXIT_FAILURE );

	    /* Use foreground from profie 0 */
	    VTFG( r, i ) = VTFG( r, 0 );
	    VTFG_FADE( r, i ) = VTFG_FADE( r, 0 );

#ifdef XFT_SUPPORT
	    VTXFTFG( r, i ) = VTXFTFG( r, 0 );
	    VTXFTFG_FADE( r, i ) = VTXFTFG_FADE( r, 0 );
#endif
	}

	/* background color of i terminal */
	if( rxvt_parse_alloc_color(r, &xcol, r->h->rs[vtbg]) )
	{
	    VTBG(r, i) = xcol.pixel;

#ifdef XFT_SUPPORT
	    rxvt_alloc_xft_color( r, &xcol, &(VTXFTBG(r, i)) );
	    rxvt_fade_color( r, &xcol, &VTBG_FADE(r, i), &VTXFTBG_FADE(r, i) );
#else
	    rxvt_fade_color( r, &xcol, &VTBG_FADE(r, i), NULL );
#endif /* XFT_SUPPORT */
	}

	else
	{
	    rxvt_msg (DBG_ERROR, DBG_INIT,  "Could not alloc background color of profile %d",
		    i );
	    if( i == 0 )
		/* Need default fg/bg */
		exit( EXIT_FAILURE );

	    /* Use background from profie 0 */
	    VTBG( r, i ) = VTBG( r, 0 );
	    VTBG_FADE( r, i ) = VTBG_FADE( r, 0 );

#ifdef XFT_SUPPORT
	    VTXFTBG( r, i ) = VTXFTBG( r, 0 );
	    VTXFTBG_FADE( r, i ) = VTXFTBG_FADE( r, 0 );
#endif
	}
    }

    /* Set foreground / background colors */
    r->pixColorsFocus[ Color_fg ] = VTFG( r, 0 );
    r->pixColorsFocus[ Color_bg ] = VTBG( r, 0 );

    if( r->TermWin.fade )
    {
	r->pixColorsUnfocus[ Color_fg ] = VTFG_FADE( r, 0 );
	r->pixColorsUnfocus[ Color_bg ] = VTBG_FADE( r, 0 );
    }

#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
    {
	r->xftColorsFocus[ Color_fg ] = VTXFTFG( r, 0 );
	r->xftColorsFocus[ Color_bg ] = VTXFTBG( r, 0 );

	if( r->TermWin.fade )
	{
	    r->xftColorsUnfocus[ Color_fg ] = VTXFTFG( r, 0 );
	    r->xftColorsUnfocus[ Color_bg ] = VTXFTBG( r, 0 );
	}
    }
#endif

    r->fgbg_tabnum = -1;    /* fg/bg corresponds to profile 0, not any
			       particular tab during initialization. */


    /*
     * Allocate generic colors.
     */
    for (i = minCOLOR; i < (XDEPTH <= 2 ? 2 : NRS_COLORS); i++)
    {
	XColor		xcol;

	if( IS_NULL(r->h->rs[Rs_color + i]) )
	    continue;

	if( !rxvt_parse_alloc_color(r, &xcol, r->h->rs[Rs_color + i]) )
	{
	    if( r->h->rs[Rs_color+i] != def_colorName[i] )
	    {
		rxvt_msg (DBG_ERROR, DBG_INIT,  "Could not allocate color '%s'\n",
			r->h->rs[Rs_color + i] );

		/* Try again with default color */
		r->h->rs[Rs_color + i] = def_colorName[i];
		i--;
		continue;
	    }

	    /*
	     * Unable to alloc even the default color. Fall back to fg/bg.
	     */
	    switch( i )
	    {
#ifndef NO_CURSORCOLOR
		case Color_cursor2:
#endif /* !NO_CURSORCOLOR */
		case Color_pointer:
		    rxvt_copy_color( r, i, Color_fg );
		    break;

		default:
		    rxvt_copy_color( r, i, Color_bg );
		    break;
	    }
	}

	else
	{
	    /*
	     * Succeeded allocating the color. Store it in pixColors.
	     */
	    rxvt_set_color( r, i, &xcol );
	}
    } /* for(i) */


    /*
     * Allocate colors which are essential if they have not been allocated.
     */
    if( XDEPTH <= 2 || !ISSET_PIXCOLOR( r->h, Color_pointer ) )
    {
	/*
	 * NOTE: Fading should be disabled for low depths. And the pointer color
	 * should not be faded.
	 */
	r->pixColorsFocus[Color_pointer]    = VTFG(r, 0);
	if( r->TermWin.fade )
	    r->pixColorsUnfocus[Color_pointer]  = VTFG(r, 0);
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	{
	    r->xftColorsFocus[Color_pointer]	= VTXFTFG(r, 0);
	    if( r->TermWin.fade )
		r->xftColorsUnfocus[Color_pointer]  = VTXFTFG(r, 0);
	}
#endif
	SET_PIXCOLOR( r->h, Color_pointer );
    }

    if( XDEPTH <= 2 || !ISSET_PIXCOLOR( r->h, Color_border ) )
	rxvt_copy_color( r, Color_border, Color_fg );


    /*
     * get scrollBar/menuBar shadow colors
     *
     * The calculations of topShadow/bottomShadow values are adapted from the
     * fvwm window manager.
     */
#ifdef KEEP_SCROLLCOLOR
    if (XDEPTH <= 2)	    /* Monochrome */
    {
	rxvt_copy_color( r, Color_scroll,	Color_fg );
	rxvt_copy_color( r, Color_topShadow,	Color_bg );
	rxvt_copy_color( r, Color_bottomShadow, Color_bg );
    }

    else
    {
	XColor		xcol[3];
	/*
	 * xcol[0] == white
	 * xcol[1] == top shadow
	 * xcol[2] == bot shadow
	 */

	xcol[1].pixel = r->pixColorsFocus[Color_scroll];
# ifdef PREFER_24BIT
	xcol[0].red = xcol[0].green = xcol[0].blue = 0xffffu;
	rxvt_alloc_color( r, &(xcol[0]), "White" );
	XQueryColors(r->Xdisplay, XCMAP, &(xcol[1]), 1);
# else
	xcol[0].pixel = WhitePixel(r->Xdisplay, XSCREEN);
	XQueryColors(r->Xdisplay, XCMAP, xcol, 2);
# endif

	/* bottomShadowColor */
	xcol[2].red	= xcol[1].red	/ 2;
	xcol[2].green	= xcol[1].green / 2;
	xcol[2].blue	= xcol[1].blue	/ 2;
	if( !rxvt_alloc_color( r, &(xcol[2]), "Color_bottomShadow" ) )
	    rxvt_copy_color( r, Color_bottomShadow, Color_Black );

	else
	    rxvt_set_color( r, Color_bottomShadow, &xcol[2] );


	/* topShadowColor */
	xcol[1].red	= max((xcol[0].red   / 5), xcol[1].red	);
	xcol[1].green	= max((xcol[0].green / 5), xcol[1].green);
	xcol[1].blue	= max((xcol[0].blue  / 5), xcol[1].blue	);
	xcol[1].red	= min(xcol[0].red,   (xcol[1].red   * 7) / 5);
	xcol[1].green	= min(xcol[0].green, (xcol[1].green * 7) / 5);
	xcol[1].blue	= min(xcol[0].blue,  (xcol[1].blue  * 7) / 5);

	if( !rxvt_alloc_color(r, &(xcol[1]), "Color_topShadow") )
	    rxvt_copy_color( r, Color_topShadow, Color_White );
	else
	    rxvt_set_color( r, Color_topShadow, &xcol[1] );

    }
#endif	/* KEEP_SCROLLCOLOR */


#ifdef TEXT_SHADOW
    if (r->h->rs[Rs_textShadow])
    {
	XColor	xcol;
	if( rxvt_parse_alloc_color( r, &xcol, r->h->rs[Rs_textShadow] ) )
	{
	    r->TermWin.shadow = xcol.pixel;
# ifdef XFT_SUPPORT
	    rxvt_alloc_xft_color( r, &xcol, &(r->TermWin.xftshadow));
# endif
	}
	else
	{
	    r->TermWin.shadow = r->pixColorsFocus[Color_Black];
# ifdef XFT_SUPPORT
	    r->TermWin.xftshadow = r->xftColorsFocus[Color_Black];
# endif
	}
    }
#endif
}