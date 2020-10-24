 * EXT: button 2 release
 */
/* EXTPROTO */
void
rxvt_selection_request(rxvt_t* r, int page, Time tm, int x, int y)
{
    rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_request %d (%lu, %d, %d)\n", page, tm, x, y ));

    if (x < 0 || x >= VT_WIDTH(r) || y < 0 || y >= VT_HEIGHT(r))
	return;		/* outside window */

    if( SEL(r).text != NULL )	    /* internal selection */
    {
	rxvt_dbgmsg ((DBG_DEBUG, DBG_SCREEN, "rxvt_selection_request %d: pasting internal\n", page ));
	rxvt_paste_str( r, page, SEL(r).text, SEL(r).len );
	return;
    }
    else
    {
	int	     i;

	r->h->selection_request_time = tm;
	r->h->selection_wait = Sel_normal;

	for (i = Sel_Primary; i <= Sel_Clipboard; i++)
	{
#ifdef MULTICHAR_SET
	    r->h->selection_type = Sel_CompoundText;
#else
	    r->h->selection_type = 0;
#endif
	    if (rxvt_selection_request_other(r, page,
#ifdef MULTICHAR_SET
		 r->h->xa[XA_COMPOUND_TEXT],
#else
		 XA_STRING,
#endif
		 i))
	    return;
	}
    }

    /* don't loop in rxvt_selection_paste() */
    r->h->selection_wait = Sel_none;