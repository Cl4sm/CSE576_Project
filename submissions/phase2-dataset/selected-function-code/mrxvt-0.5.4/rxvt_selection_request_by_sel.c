 * EXT: button 2 release
 */
/* EXTPROTO */
void
rxvt_selection_request_by_sel(rxvt_t* r, int page, Time tm, int x, int y,int sel)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_request %d (%lu, %d, %d)\n", page, tm, x, y ));

    if (x < 0 || x >= VT_WIDTH(r) || y < 0 || y >= VT_HEIGHT(r))
	return;		/* outside window */

	r->h->selection_request_time = tm;
	r->h->selection_wait = Sel_normal;

#ifdef MULTICHAR_SET
   r->h->selection_type = Sel_CompoundText;
#else
   r->h->selection_type = 0;
#endif
   rxvt_selection_request_other(r, page,
#ifdef MULTICHAR_SET
	 r->h->xa[XA_COMPOUND_TEXT],
#else
	 XA_STRING,