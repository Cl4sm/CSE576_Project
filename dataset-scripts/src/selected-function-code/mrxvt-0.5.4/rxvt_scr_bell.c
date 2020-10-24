/* ------------------------------------------------------------------------- */
/* EXTPROTO */
void
rxvt_scr_bell(rxvt_t *r, int page)
{
#ifndef NO_BELL

#if defined(THROTTLE_BELL_MSEC) && THROTTLE_BELL_MSEC > 0
    /* Maximal number of bell per pre-defined time interval */
    static int		    bellcount	= 0;
    static struct timeval   lastBell	= {0, 0};
    struct timeval	    tvnow	= {0, 0};
    long		    tminterval;

#ifdef HAVE_NANOSLEEP
    struct timespec	    rqt;

    rqt.tv_sec = r->TermWin.vBellDuration / 1000000000ul;
    rqt.tv_nsec = r->TermWin.vBellDuration % 1000000000ul;
#endif

    if (gettimeofday (&tvnow, NULL) >= 0)
    {
	if (0 == lastBell.tv_sec && 0 == lastBell.tv_usec)
	    /* first time bell, try avoid integer overflow */
	    tminterval = 0;

	else
	    tminterval = (tvnow.tv_sec - lastBell.tv_sec) * 1000 +
			(tvnow.tv_usec - lastBell.tv_usec) / 1000;

	lastBell = tvnow;
	if (tminterval > THROTTLE_BELL_MSEC)
	    bellcount = 1;

	else if (bellcount ++ >= THROTTLE_BELL_COUNT)
	    return;
    }
#endif	/* THROTTLE_BELL_MSEC && THROTTLE_BELL_MSEC > 0 */

# ifndef NO_MAPALERT
#  ifdef MAPALERT_OPTION
    if (ISSET_OPTION(r, Opt_mapAlert))
#  endif
	XMapWindow(r->Xdisplay, r->TermWin.parent);
# endif
    if(
	    ISSET_OPTION(r, Opt_visualBell) ||
	    (
	     ISSET_OPTION( r, Opt_currentTabVBell ) && APAGE(r) == page
	     && r->TermWin.focus
	    )
      )
    {
	/*
	 * Visual bells don't need to be rung on windows which are not visible.
	 */
	if( APAGE(r) != page  || r->h->refresh_type == NO_REFRESH )
	    return;

#if defined(TRANSPARENT) || defined(BACKGROUND_IMAGE)
	/*
	 * Reverse video bell doesn't look so good with transparency or a
	 * background pixmap. Flash screen ourselves.
	 */
	if (
# ifdef TRANSPARENT
	      r->h->am_transparent || r->h->am_pixmap_trans
#  ifdef BACKGROUND_IMAGE
	      ||
#  endif
# endif
# ifdef BACKGROUND_IMAGE
	      IS_PIXMAP(PVTS(r, page)->pixmap)
# endif
	   )
	{
	    XGCValues values;

	    XGetGCValues( r->Xdisplay, r->TermWin.gc,
		    GCForeground | GCFillStyle, &values);

	    XSetForeground( r->Xdisplay, r->TermWin.gc,
		    r->pixColors[Color_fg] );
	    XSetFillStyle( r->Xdisplay, r->TermWin.gc, FillSolid);

	    XFillRectangle( r->Xdisplay, PVTS(r, page)->vt, r->TermWin.gc,
		    Row2Pixel(0), Col2Pixel(0),
		    Width2Pixel( r->TermWin.ncol),
		    Height2Pixel( r->TermWin.nrow) );

	    XChangeGC( r->Xdisplay, r->TermWin.gc,
		    GCForeground | GCFillStyle, &values);

	    XSync( r->Xdisplay, False);

#ifdef HAVE_NANOSLEEP
	    if( r->TermWin.vBellDuration )
		nanosleep(&rqt, NULL);
#endif

	    XClearArea( r->Xdisplay, PVTS(r, page)->vt, 0, 0, 0, 0, True);
	}
	else
#endif /* TRANSPARENT || BACKGROUND_IMAGE */
	{
	    /* refresh also done */
	    rxvt_scr_rvideo_mode(r, page, !PVTS(r, page)->rvideo);

#ifdef HAVE_NANOSLEEP
	    rxvt_scr_refresh( r, page,  r->h->refresh_type );
	    XSync( r->Xdisplay, False );
	    if( r->TermWin.vBellDuration )
		nanosleep(&rqt, NULL);
#endif
	    rxvt_scr_rvideo_mode(r, page, !PVTS(r, page)->rvideo);
	}
    }

    else if( r->h->rs[Rs_bellCommand] && *r->h->rs[Rs_bellCommand] )
	rxvt_async_exec( r, r->h->rs[Rs_bellCommand] );

    else
	XBell(r->Xdisplay, 0);