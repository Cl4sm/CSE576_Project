void
rxvt_tabbar_dispatcher (rxvt_t* r, XButtonEvent* ev)
{
    register int    x, y, z, but;

    x = ev->x;
    y = ev->y;
    but = -1;
    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR, "click in (%d,%d)\n", x, y));

    /* Button4/Button5 of wheel mouse activate the left/right tab as
       Mozilla firefox */
    switch ( ev->button )
    {
#ifdef HAVE_MENUBAR
	case Button3:
	    if( r->h->popupMenu[0] )
	    {
		int	x, y;
		Window	unused_cr;

		r->h->showingMenu |= POPUP_MENU;

		XTranslateCoordinates( r->Xdisplay, ev->window,
			r->TermWin.parent, ev->x, ev->y, &x, &y, &unused_cr);

		r->h->ActiveMenu = r->h->popupMenu[0];

		r->h->ActiveMenu->x = x;
		r->h->ActiveMenu->y = y;

		XDefineCursor(r->Xdisplay, AVTS(r)->vt, r->h->bar_pointer);
		rxvt_menu_show(r);
		return;
	    }
	    break;
#endif
	case Button4: /* activate left tab */
	    if (0 != ATAB(r))
		rxvt_activate_page (r, ATAB(r)-1);
	    else if (0 != LTAB(r))
		rxvt_activate_page (r, LTAB(r));
	    return;

	case Button5: /* activate right tab */
	    if( ATAB(r) != LTAB(r) )
		rxvt_activate_page( r, ATAB(r) + 1 );
	    else if( 0 != LTAB(r) )
		rxvt_activate_page( r, 0 );
	    return;

	default:
	    break;
    }


    /* let's decode where the user click */
    z = TWIN_WIDTH(r) - x;
    if (
	    NOTSET_OPTION(r, Opt2_hideButtons)
	    && z < 4*(BTN_WIDTH+BTN_SPACE)
	    && (z%(BTN_WIDTH+BTN_SPACE)) > BTN_SPACE
       )
    {
	but = z/(BTN_WIDTH+BTN_SPACE);

	/* we should only handle left-mouse-button clicks */
	if ( ev->button != Button1 )
	{
	    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"skip non-left-mouse-button click\n"));
	    return;
	}

	rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"click on button %d\n",but));
	switch(but)
	{
	    case 0 : /* right shift */
		if (r->tabBar.atab < LTAB(r))
		    rxvt_activate_page (r, r->tabBar.atab+1);
		break;

	    case 1 : /* left shift */
		if (r->tabBar.atab > 0)
		    rxvt_activate_page (r, r->tabBar.atab-1);
		break;

	    case 2 : /* delete the active vt if it's in primary screen */
		if(
		    NOTSET_OPTION(r, Opt2_protectSecondary) ||
		    (
		       ISSET_OPTION(r, Opt2_protectSecondary) &&
		       PRIMARY == AVTS(r)->current_screen
		    )
		  )
		{
		    /*
		     * 2006-09-19 gi1242: If user presses the X button, don't
		     * hold the tab open, regardless of exit status.
		     */
		    AVTS(r)->holdOption &= ~(HOLD_NORMALBIT|HOLD_STATUSBIT);
		    rxvt_kill_page (r, ATAB(r));
		}
		break;

	    case 3 : /* create a new vt*/
		rxvt_append_page (r, 0, NULL, NULL);
		break;

	    default :
		break;
	}
    }
    else if ( x < TAB_SPACE && LTAB(r) >= 0)
    {
	register int	w = 0;
	register int	i;
	for ( i = FVTAB(r); w < x && i <= LVTAB(r); i++)
	    w += TAB_WIDTH(i);

	if( w - TAB_BORDER >= x )
	{
	    but = i - 1;

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_TABBAR,"click on tab %d\n", but));
	    switch( ev->button )
	    {
		case Button1:
		    /* activate the selected tab */
		    rxvt_activate_page (r, but);
		    r->tabClicked = but;
		    break;

		case Button2:
		    /* change tab title on middle click */
		    if (NULL != r->selection.text)
			rxvt_tabbar_set_title (r, but, r->selection.text);
		    break;
	    }
	}
	else
	{
	    /* change tab title of active tab on middle click */
	    if ((Button2 == ev->button) && (NULL != r->selection.text))
		rxvt_tabbar_set_title (r, ATAB(r), r->selection.text);
	}
    }
}