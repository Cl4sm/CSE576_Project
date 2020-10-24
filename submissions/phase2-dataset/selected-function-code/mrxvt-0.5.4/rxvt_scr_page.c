 */
/* EXTPROTO */
int
rxvt_scr_page(rxvt_t* r, int page, enum page_dirn direction, int nlines)
{
    int		n;
    uint16_t	oldviewstart;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_scr_page %d (%s, %d) view_start:%d\n", page, ((direction == UP) ? "UP" : "DN"), nlines, VSTART));

    oldviewstart = VSTART;
    if (direction == UP)
    {
	n = VSTART + nlines;
	VSTART = min(n, PVTS(r, page)->nscrolled);
    }
    else
    {
	n = VSTART - nlines;
	VSTART = max(n, 0);
    }