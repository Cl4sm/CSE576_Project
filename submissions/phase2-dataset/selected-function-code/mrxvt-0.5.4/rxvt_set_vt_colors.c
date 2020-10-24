int
rxvt_set_vt_colors( rxvt_t *r, int page )
{
    int		    changed = 0;
    int		    useFocusColors;
    unsigned long   *pix_colors;

    rxvt_dbgmsg(( DBG_DEBUG, DBG_INIT, "%s(r, page=%d). ", __func__, page ));

    useFocusColors = ( r->TermWin.focus || !r->TermWin.fade );
    pix_colors = (useFocusColors ? r->pixColorsFocus : r->pixColorsUnfocus);
    if( r->pixColors != pix_colors )
    {
	changed		= 1;
	r->pixColors	= pix_colors;
    }

#ifdef XFT_SUPPORT
    if( ISSET_OPTION( r, Opt_xft ) )
	r->xftColors = useFocusColors ? r->xftColorsFocus : r->xftColorsUnfocus;
#endif

    if( rxvt_set_fgbg_colors( r, page ) )
	changed = 1;

    if( changed )
    {
	/*
	 * Set foreground/background color for GC. This is necessary. Since all
	 * VTs share the same GC, if we do not set the color here, color from
	 * other VTs will be used to draw the following text till there is a
	 * color change.
	 */
	XSetForeground( r->Xdisplay, r->TermWin.gc, r->pixColors[Color_fg] );
	XSetBackground( r->Xdisplay, r->TermWin.gc, r->pixColors[Color_bg] );

	if( IS_WIN( PVTS(r, page)->vt ) )
# ifdef TRANSPARENT
	    if (NOTSET_OPTION(r, Opt_transparent))
# endif	/* TRANSPARENT */
#ifdef BACKGROUND_IMAGE
		if (NOT_PIXMAP(PVTS(r, page)->pixmap))
#endif	/* BACKGROUND_IMAGE */
		    XSetWindowBackground(r->Xdisplay, PVTS(r, page)->vt,
			r->pixColors[Color_bg]);
    }

    rxvt_dbgmsg(( DBG_DEBUG, DBG_INIT, "Returning %d\n", changed ));
    return changed;
}