void
rxvt_scr_rvideo_mode(rxvt_t* r, int page, int mode)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "%s(r, page=%d, mode=%d)\n", __func__, page, mode ));

    if (PVTS(r, page)->rvideo != mode)
    {
	PVTS(r, page)->rvideo = mode;

	SWAP_IT( PVTS(r, page)->p_fg, PVTS(r, page)->p_bg, unsigned long );
#ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    SWAP_IT( PVTS(r, page)->p_xftfg, PVTS(r, page)->p_xftbg, XftColor );
#endif
	if( r->TermWin.fade )
	{
	    SWAP_IT( PVTS(r, page)->p_fgfade, PVTS(r, page)->p_bgfade,
		    unsigned long );
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		SWAP_IT( PVTS(r, page)->p_xftfgfade, PVTS(r, page)->p_xftbgfade,
			XftColor );
#endif
	}

	if( page == ATAB(r) )
	{
	    /* Background colors need to be forcibly reset */
	    r->fgbg_tabnum = -1;
	    rxvt_set_vt_colors( r, ATAB(r) );
	}

	rxvt_scr_clear( r, page );
	rxvt_scr_touch( r, page, True );
    }
}