void
rxvt_toggle_subwin( rxvt_t *r, const unsigned char *str)
{
    if (IS_NULL(str) || (char) 0 == *str ||
	    STRCHR( str, 't') || STRCHR( str, 'T' ) )
    {
	/*
	 * Backward compatibility -- If str is NULL or empty, then toggle the
	 * tabbar visibility.
	 */
	if (rxvt_tabbar_visible (r) &&
	    (IS_NULL(str) || '+' != *str))
	{
	    /*
	     * If the tabbar is visible, and we are not forced to show it then
	     * hide it.
	     */
	    if( rxvt_tabbar_hide(r) ) rxvt_resize_on_subwin (r, HIDE_TABBAR);
	}
	else if (!rxvt_tabbar_visible (r) &&
	    (IS_NULL(str) || '-' != *str))
	{
	    /*
	     * If the tabbar is hidden, and we are not forced to hide it, then
	     * show the tabbar.
	     */
	    if( rxvt_tabbar_show(r)) rxvt_resize_on_subwin (r, SHOW_TABBAR);
	}

	return;
    }

    /*
     * The remainder of this function assumes a non-empty string.
     */
    if( STRCHR( str, 'b') || STRCHR( str, 'B') )
    {
	/* Toggle tabbar buttons */
	switch( *str )
	{
	    case '+':
		/* Show buttons */
		UNSET_OPTION(r, Opt2_hideButtons);
		break;

	    case '-':
		/* Hide buttons */
		SET_OPTION(r, Opt2_hideButtons);
		break;

	    default:
		/* Toggle buttons */
		TOGGLE_OPTION(r, Opt2_hideButtons);
	}

	/* Refresh tabbar */
	rxvt_tabbar_set_visible_tabs (r, False);
	if (IS_WIN(r->tabBar.win))
	    XClearArea( r->Xdisplay, r->tabBar.win, 0, 0, 0, 0, True);

	return;
    }


#ifdef HAVE_SCROLLBARS
    if( STRCHR( str, 's') || STRCHR( str, 'S' ) )
    {
	if( rxvt_scrollbar_visible (r) && *str != '+' )
	{
	    if( rxvt_scrollbar_hide(r) )
		rxvt_resize_on_subwin (r, HIDE_SCROLLBAR);
	}
	else if( !rxvt_scrollbar_visible( r ) && *str != '-' )
	{
	    if( rxvt_scrollbar_show(r) )
		rxvt_resize_on_subwin (r, SHOW_SCROLLBAR);
	}

	return;
    }
#endif

#ifdef HAVE_MENUBAR
    if( STRCHR( str, 'm') || STRCHR( str, 'M' ) )
    {
	if( rxvt_menubar_visible (r) && *str != '+' )
	{
	    if( rxvt_menubar_hide(r) )
		rxvt_resize_on_subwin (r, HIDE_MENUBAR);
	}
	else if( !rxvt_menubar_visible( r ) && *str != '-' )
	{
	    if( rxvt_menubar_show(r) )
		rxvt_resize_on_subwin (r, SHOW_MENUBAR);
	}

	return;
    }
#endif
}