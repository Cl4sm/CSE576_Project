int
rxvt_calc_colrow (rxvt_t* r, unsigned int width, unsigned int height)
{
    unsigned int    ncol, nrow;


    assert (0 != r->TermWin.fwidth);
    assert (0 != r->TermWin.fheight);

    /* Do nothing if size does not change, unless we requested the resize. */
    if ( !r->h->want_resize &&
	    r->szHint.width == width && r->szHint.height == height)
	return 0;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Recalc row/col of (%d, %d)\n", width, height));
    ncol = Pixel2Width (width - r->szHint.base_width);
    nrow = Pixel2Height (height - r->szHint.base_height);
    MAX_IT(ncol, 1);
    MAX_IT(nrow, 1);

    r->h->prev_ncol = r->TermWin.ncol;
    r->h->prev_nrow = r->TermWin.nrow;
    r->TermWin.ncol = ncol;
    r->TermWin.nrow = nrow;

    /* 
     * According to X Reference, we should only trust the size of a Window
     * through ConfigureNotify event, or through XGet... functions. This
     * function should only be called from ConfigureNotify event handler
     */
    r->szHint.width = width;
    r->szHint.height = height;

    return ((r->h->prev_ncol != r->TermWin.ncol) ||
	    (r->h->prev_nrow != r->TermWin.nrow));
}