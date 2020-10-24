void
rxvt_remove_page (rxvt_t* r, short page)
{
    register int    i;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_TABBAR,"remove_page(%d)\n", page));


    /* clean utmp/wtmp entry */
#ifdef UTMP_SUPPORT
    rxvt_privileges (RESTORE);
    rxvt_cleanutent (r, page); 
    rxvt_privileges (IGNORE);
#endif

    /* free virtual terminal related resources */
    assert (PVTS(r, page)->ttydev);
    rxvt_free (PVTS(r, page)->ttydev);
    assert (PVTS(r, page)->cmd_fd >= 0);
    close (PVTS(r, page)->cmd_fd);

    if (PVTS(r, page)->v_buffer)
    {
	rxvt_free (PVTS(r, page)->v_buffer);
	PVTS(r, page)->v_buffer = NULL;
    }

    /* free screen structure */
    rxvt_scr_release (r, page);

    /* destroy the virtual terminal window */
    rxvt_destroy_termwin (r, page);

    /* update total number of tabs */
    LTAB(r)--;

    /* quit the last the terminal, exit the application */
    if( LTAB(r) < 0 )
	rxvt_clean_exit (r);

    /* update TermWin and tab_widths */
    for (i = page; i <= LTAB(r); i++)
    {
	PVTS(r, i) = PVTS(r, i+1);
	refresh_tabbar_tab( r, i);
    }

    /* update selection */
    if (page == r->selection.vt)
	rxvt_process_selectionclear (r, page);
    else if (r->selection.vt > page)
	r->selection.vt --;

    /*
     * Now we try to set correct atab, ptab, fvtab, and lvtab
     * Must be careful here!!!
     */
    /* update previous active tab */
    if (PTAB(r) > page) PTAB(r)--;
    /* in case PTAB is invalid */
    if (PTAB(r) > LTAB(r)) PTAB(r) = LTAB(r);

    /* update active tab */
    if( ATAB(r) == page )
    {
	/* Fall back to previous active */
	ATAB(r) = PTAB(r);

	/* Make the previous active tab the previous / next tab if possible. */
	if( PTAB(r) > 0 ) PTAB(r)--;
	else if (PTAB(r) < LTAB(r) ) PTAB(r)++;
    }
    else if( ATAB(r) > page) ATAB(r)--;

    /* always set mapped flag */
    AVTS(r)->mapped = 1;

    /* Adjust the number of FD's we select() for.  */
    rxvt_adjust_fd_number(r);


    /* adjust visible tabs */
    rxvt_tabbar_set_visible_tabs (r, True);
    refresh_tabbar_tab( r, ATAB(r));	/* Active tab has changed */

    /* redraw the tabs and buttons */
    if (r->tabBar.state)
    {
	if( LTAB(r) == 0 && ISSET_OPTION(r, Opt2_autohideTabbar) 
		&& rxvt_tabbar_hide( r ))
	    /*
	     * Only one tab left. Auto hide tabbar.
	     */
	    rxvt_resize_on_subwin (r, HIDE_TABBAR);
    }

    /* Switch fg/bg colors */
    rxvt_set_vt_colors( r, ATAB(r) );
    XMapRaised( r->Xdisplay, AVTS(r)->vt );

    /*
     * We don't need to touch the screen here. XMapRaised will generate a
     * MapNotify and Expose events, which will refresh the screen as needed.
     * Touching the screen unnecessarily causes a flicker (and is *horrible*
     * under slow connections).
     */
    /* rxvt_scr_touch (r, ATAB(r), True); */

    /* synchronize terminal title with tab title */
    if (ISSET_OPTION(r, Opt2_syncTabTitle))
	sync_tab_title( r, ATAB(r) );

    /* synchronize icon name to tab title */
    if (ISSET_OPTION(r, Opt2_syncTabIcon))
	rxvt_set_icon_name(r,
		(const unsigned char*) PVTS(r, ATAB(r))->tab_title);
}