void
rxvt_set_window_color(rxvt_t* r, int page, int idx, const char *color)
{
    XColor	    xcol;
    int		    color_set;
    register int    i;


    rxvt_dbgmsg ((DBG_DEBUG, DBG_MAIN, "%s( r, %d, %d, %s), ATAB=%d\n", __func__, page, idx, color, ATAB(r)));

    if (IS_NULL(color) || (char) 0 == *color)
	return;

    /*
     * Set the fg/bg colors from this page, just in case the fg/bg is to be
     * changed.
     */
    rxvt_set_fgbg_colors( r, page );

    color_set = ISSET_PIXCOLOR(r->h, idx);

    /* handle color aliases */
    if( isdigit((int) *color) )
    {
	i = atoi(color);
	if (i >= 8 && i <= 15)		/* bright colors */
	{
	    i -= 8;
# ifndef NO_BRIGHTCOLOR
	    rxvt_copy_color( r, idx, minBrightCOLOR + i );
	    goto Done;
# endif
	}
	if (i >= 0 && i <= 7)		/* normal colors */
	{
	    rxvt_copy_color( r, idx, minCOLOR + i );
	    goto Done;
	}
    }

    if( !rxvt_parse_alloc_color(r, &xcol, color) )
	/* Could not alloc color */
	return;

    /* XStoreColor (r->Xdisplay, XCMAP, XColor*); */

    /*
     * FIXME: should free colors here, but no idea how to do it so instead,
     * so just keep gobbling up the colormap
     */
# if 0 /*{{{*/
    for (i = Color_Black; i <= Color_White; i++)
	if (r->pixColors[idx] == r->pixColors[i])
	    break;

    if (i > Color_White)
    {
	fprintf (stderr, "XFreeColors: r->pixColors [%d] = %lu\n",
		idx, r->pixColors [idx]);
	XFreeColors(r->Xdisplay, XCMAP, (r->pixColors + idx), 1,
		DisplayPlanes(r->Xdisplay, XSCREEN));
    }
# endif /*}}}*/

    rxvt_set_color( r, idx, &xcol );

Done:
    if( idx == Color_bg )
    {
	PVTS( r, page )->p_bg	= r->pixColorsFocus[idx];
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    PVTS( r, page )->p_xftbg = r->xftColorsFocus[idx];
#endif

	if( r->TermWin.fade )
	{
	    PVTS( r, page )->p_bgfade	= r->pixColorsUnfocus[idx];
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		PVTS( r, page )->p_xftbgfade = r->xftColorsUnfocus[idx];
#endif
	}

	/*
	 * Update the GC / window background if necessary.
	 */
	if( page == ATAB(r) )
	    r->fgbg_tabnum = -1;
    }

    else if ( idx == Color_fg )
    {
	PVTS( r, page )->p_fg	= r->pixColorsFocus[idx];
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    PVTS( r, page )->p_xftfg = r->xftColorsFocus[idx];
#endif

	if( r->TermWin.fade )
	{
	    PVTS( r, page )->p_fgfade	= r->pixColorsUnfocus[idx];
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		PVTS( r, page )->p_xftfgfade = r->xftColorsUnfocus[idx];
#endif
	}

	if( page == ATAB(r) )
	    /* Force rxvt_set_vt_colors() to update the GC / background. */
	    r->fgbg_tabnum = -1;
    }

    /* handle Color_BD, scrollbar background, etc. */
#if 0 /*{{{*/
    /*
     * Setting the environment will not propogate to an already running child
     * process.
     */
    rxvt_set_colorfgbg(r);
#endif /*}}}*/
    if( idx == Color_pointer )
	rxvt_recolour_cursor(r);

#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
# ifdef TINTING_SUPPORT
    if (idx == Color_tint)
    {
#  ifdef TRANSPARENT
	if (ISSET_OPTION(r, Opt_transparent))
	    /* reset background */
	    rxvt_check_our_parents (r);
	else
#  endif
#  ifdef BACKGROUND_IMAGE
	{   /* reset background */
	    for (i = 0; i <= LTAB(r); i ++)
		rxvt_resize_pixmap (r, i);
	}
#  endif
	{   /* empty body to suppress compile error */	}
    }
# endif	/* TINTING_SUPPORT */
#endif	/* TRANSPARENT || BACKGROUND_IMAGE */

    /*
     * Restore the fg/bg colors from the active tab. Call rxvt_set_vt_colors
     * instead of rxvt_set_fgbg_colors, so that the GC fgbg & window background
     * will be refreshed if necessary.
     */
    rxvt_set_vt_colors( r, ATAB(r) );

    /*
     *  If our palette has changed, the screen must be refreshed.
     */
    XClearArea( r->Xdisplay, AVTS(r)->vt, 0, 0, 0, 0, True );
}