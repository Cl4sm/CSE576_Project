void
rxvt_pre_show_init( rxvt_t *r )
{
    /*
     * 2006-08-18 gi1242 TODO: If we're using Xft, then we don't need to
     * initialize this array.
     */
    r->pixColorsFocus = rxvt_malloc( sizeof(unsigned long) * (TOTAL_COLORS));
    r->pixColors = r->pixColorsFocus;


#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
    {
	r->xftColorsFocus = rxvt_malloc( sizeof(XftColor) * (TOTAL_COLORS) );
	r->xftColors = r->xftColorsFocus;
    }
    else
    {
	SET_NULL( r->xftColors );
	SET_NULL( r->xftColorsFocus );
    }
#endif

    if( r->TermWin.fade )
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_MAIN, "Allocating space for fade colors\n"));
	r->pixColorsUnfocus =
	    rxvt_malloc( sizeof(unsigned long) * (TOTAL_COLORS) );

# ifdef XFT_SUPPORT
	if( ISSET_OPTION( r, Opt_xft ) )
	    r->xftColorsUnfocus =
		rxvt_malloc( sizeof(XftColor) * TOTAL_COLORS );
	else
	    SET_NULL( r->xftColorsUnfocus );
# endif /* XFT_SUPPORT */
    }

    else
    {
	SET_NULL( r->pixColorsUnfocus );
# ifdef XFT_SUPPORT
	SET_NULL( r->xftColorsUnfocus );
# endif
    }
}