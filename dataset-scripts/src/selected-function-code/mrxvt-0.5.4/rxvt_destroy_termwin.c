void
rxvt_destroy_termwin( rxvt_t *r, int page )
{
    assert (page < MAX_PAGES);
    assert (PVTS(r, page)->tab_title);

    rxvt_free (PVTS(r, page)->tab_title);
    SET_NULL(PVTS(r, page)->tab_title);

    rxvt_free( PVTS(r, page)->title_format );
    SET_NULL( PVTS(r, page)->title_format );

#ifdef XFT_SUPPORT
    if (ISSET_OPTION(r, Opt_xft))
    {
	if (PVTS(r, page)->xftvt)
	    XftDrawDestroy (PVTS(r, page)->xftvt);
	SET_NULL(PVTS(r, page)->xftvt);
    }
#endif
    assert (IS_WIN(PVTS(r, page)->vt));
    XDestroyWindow (r->Xdisplay, PVTS(r, page)->vt);
    UNSET_WIN(PVTS(r, page)->vt);

#ifdef BACKGROUND_IMAGE
    if (IS_PIXMAP(PVTS(r, page)->pixmap))
    {
	XFreePixmap (r->Xdisplay, PVTS(r, page)->pixmap);
	UNSET_PIXMAP(PVTS(r, page)->pixmap);
    }
    if (IS_PIXMAP(PVTS(r, page)->bg.pixmap))
    {
	XFreePixmap (r->Xdisplay, PVTS(r, page)->bg.pixmap);
	UNSET_PIXMAP(PVTS(r, page)->bg.pixmap);
    }
#endif

    /* Set vterm index to -1, so that we know it's unused */
    PVTS(r, page)->vts_idx = -1;
}