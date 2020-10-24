void
rxvt_exit_request( rxvt_t *r )
{
    /* Avoid exiting if there are multiple tabs with hidden tabbar */
    if( LTAB(r) > 0 &&  !rxvt_tabbar_visible( r ) )
    {
	XBell( r->Xdisplay, 0 );
	if( rxvt_tabbar_show(r) ) rxvt_resize_on_subwin (r, SHOW_TABBAR);
	return;
    }

    /* Avoid exiting if some tab is in the secondary screen */
    if(ISSET_OPTION(r, Opt2_protectSecondary))
    {
	int i, dontExit = 0;

	for( i=0; i <= LTAB(r); i++)
	{
	    if( PVTS(r, i)->current_screen == SECONDARY )
	    {
		dontExit = 1;
		if( i != ATAB(r) )
		    rxvt_tabbar_highlight_tab( r, i, False);
	    }
	}

	if( dontExit )
	{
	    XBell( r->Xdisplay, 0);
	    return;
	}
    }

    rxvt_close_all_tabs(r);
}