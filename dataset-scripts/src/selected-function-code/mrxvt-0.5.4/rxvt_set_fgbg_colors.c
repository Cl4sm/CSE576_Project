int
rxvt_set_fgbg_colors( rxvt_t *r, int page )
{
    int changed = 0;

    rxvt_dbgmsg(( DBG_DEBUG, DBG_INIT, "%s(r, page=%d)"
		": fgbg_tabnum=%d, globalTabNum=%d\n", __func__, page,
		r->fgbg_tabnum, PVTS(r, page)->globalTabNum));

    if(
	 r->fgbg_tabnum == PVTS(r, page)->globalTabNum &&
	 (
	   r->TermWin.fade			||
	   !ISSET_PIXCOLOR( r->h, Color_ufbg )	||
	   (
	     /*
	      * If we dont have fading, but have ufbg, then make sure that
	      * Color_bg points to the correct color.
	      */
	     r->TermWin.focus						?
		( r->pixColors[Color_bg] == PVTS(r, page)->p_bg )	:
		( r->pixColors[Color_bg] == r->pixColors[Color_ufbg] )
	   )
	 )
      )
	return 0;   /* No change */

    setChanged( r->pixColorsFocus[Color_fg], PVTS( r, page)->p_fg );
#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
	setChangedXft( r->xftColorsFocus[Color_fg], PVTS(r, page)->p_xftfg );
#endif

    if( r->TermWin.fade )
    {
	/* Ignore ufbg, and use faded colors */
	setChanged( r->pixColorsFocus[Color_bg], PVTS(r, page)->p_bg );

	setChanged( r->pixColorsUnfocus[Color_fg], PVTS(r, page)->p_fgfade );
	setChanged( r->pixColorsUnfocus[Color_bg], PVTS(r, page)->p_bgfade );

#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	{
	    setChangedXft( r->xftColorsFocus[Color_bg],
		    PVTS(r, page)->p_xftbg );

	    setChangedXft( r->xftColorsUnfocus[Color_fg],
		    PVTS(r, page)->p_xftfgfade );
	    setChangedXft( r->xftColorsUnfocus[Color_bg],
		    PVTS(r, page)->p_xftbgfade );
	}
#endif
    }

    else if( ISSET_PIXCOLOR( r->h, Color_ufbg ) && !r->TermWin.focus )
    {
	/* No fading. But use Color_ufbg */
	setChanged( r->pixColorsFocus[Color_bg],
		r->pixColorsFocus[Color_ufbg] );
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    setChangedXft( r->xftColorsFocus[Color_bg],
		    r->xftColorsFocus[Color_ufbg] );
#endif
    }

    else
    {
	/* Use fgbg from profile */
	setChanged( r->pixColorsFocus[Color_bg],
		PVTS(r, page)->p_bg );
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    setChangedXft( r->xftColorsFocus[Color_bg],
		    PVTS(r, page)->p_xftbg );
#endif
    }

    r->fgbg_tabnum = PVTS( r, page )->globalTabNum;

    rxvt_dbgmsg(( DBG_DEBUG, DBG_INIT, "%s(r, page=%d) returning %d\n",
	    __func__, page, changed ));
    return changed; /* Changed */
}