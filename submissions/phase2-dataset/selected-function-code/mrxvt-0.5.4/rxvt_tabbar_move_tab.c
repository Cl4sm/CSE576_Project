void
rxvt_tabbar_move_tab (rxvt_t* r, short newPage)
{
    short   curPage	= ATAB(r);
    short   i;

    
    if (
	    0 == LTAB(r) ||			/* Only one tab (no move
						   possible) */
	    newPage == curPage ||		/* Move to itself */
	    newPage < 0 || newPage > LTAB(r)	/* Out of range */
       )
	return;

    if( newPage < curPage )
    {
	term_t* temp_vt = r->vts[curPage];

	/* Shift pages newPage .. curPage-1 one to the right. */
	for( i = curPage; i > newPage; i--)
	    r->vts[i] = r->vts[i-1];

	r->vts[newPage] = temp_vt;

	/* Update selection */
	if( r->selection.vt >= newPage && r->selection.vt < curPage )
	    r->selection.vt++;
	else if( r->selection.vt == curPage )
	    r->selection.vt = newPage;
    }
    else
    {
	term_t* temp_vt = r->vts[curPage];

	/* Shift pages curPage+1 .. newPage one to the left. */
	for( i = curPage; i < newPage; i++)
	    r->vts[i] = r->vts[i+1];

	r->vts[newPage] = temp_vt;

	/* Update selection */
	if( r->selection.vt > curPage && r->selection.vt <= newPage)
	    r->selection.vt--;
	else if( r->selection.vt == curPage )
	    r->selection.vt = newPage;
    }

    /* adjust active tab */
    ATAB(r) = newPage;
    /* adjust previous active tab */
    if (PTAB(r) == newPage) PTAB(r) = curPage;

    /* refresh tabbar */
    if (newPage < FVTAB(r) || newPage > LVTAB(r))
	rxvt_tabbar_set_visible_tabs (r, True);
    else
    {
	/*
	 * If the width of newPage is different from that of curPage, then all
	 * tabs in between newPage and curPage will have to be refreshed.
	 */
	for( i = min( newPage, curPage ); i <= max( newPage, curPage ); i++ )
	    refresh_tabbar_tab( r, i);
    }

    if( ISSET_OPTION( r, Opt2_syncTabTitle ) )
	sync_tab_title( r, ATAB(r) );
}