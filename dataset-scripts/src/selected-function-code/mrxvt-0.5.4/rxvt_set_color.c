void
rxvt_set_color( rxvt_t *r, int cIndex, const XColor *xcol )
{
    /* xcol must contain an ALLOCATED color */
    r->pixColorsFocus[cIndex] = xcol->pixel;
#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
	rxvt_alloc_xft_color( r, xcol, &r->xftColorsFocus[cIndex] );
#endif

    if( r->TermWin.fade )
    {
	if( cIndex == Color_pointer )
	{
	    /* Don't fade these colors */
	    r->pixColorsUnfocus[cIndex] = r->pixColorsFocus[cIndex];
#ifdef XFT_SUPPORT
	    if( ISSET_OPTION( r, Opt_xft ) )
		r->xftColorsUnfocus[cIndex] = r->xftColorsFocus[cIndex];
#endif
	}

	else
	    rxvt_fade_color( r, xcol, &r->pixColorsUnfocus[cIndex],
#ifdef XFT_SUPPORT
		ISSET_OPTION(r, Opt_xft) ? &r->xftColorsUnfocus[cIndex] : NULL
#else
		NULL
#endif
		);
    }

    SET_PIXCOLOR( r->h, cIndex );
}