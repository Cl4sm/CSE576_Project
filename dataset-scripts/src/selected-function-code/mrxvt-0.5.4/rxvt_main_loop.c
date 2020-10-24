void
rxvt_main_loop(rxvt_t *r)
{
    register int	i;
    unsigned char	ch;
    int			page;


    rxvt_dbgmsg ((DBG_VERBOSE, DBG_COMMAND,  "Entering rxvt_main_loop()\n" ));

    /* Send the screen size. */
    for (i = 0; i <= LTAB(r); i ++)
    {
	rxvt_tt_winsize(PVTS(r, i)->cmd_fd,
	    r->TermWin.ncol, r->TermWin.nrow, PVTS(r, i)->cmd_pid);
    }

    while (1)
    {
	/* wait for something */
	page = -1;

	ch = 0;

	while(
	       r->ndead_childs == 0		&&	/* Nothing dead */
	       r->cleanDeadChilds == 0	&&	/* Nothing to be cleaned up */
	       ( (ch = rxvt_cmd_getc(r, &page)) == 0 )	/* No input */
	     )
	    ;

	/*
	 * 2006-08-23 gi1242: If rxvt_cmd_getc is called, and then
	 * r->ndead_childs gets set during this call, then we should not discard
	 * the return value of rxvt_cmd_getc.
	 */

	if( page != -1 && ch != 0 )
	    /* rxvt_cmd_getc() returned something */
	    rxvt_process_getc( r, page, ch );

	/*
	 * See if we need a refresh. If we read a large number of bytes from
	 * this tabs cmd_fd, then we should let rxvt_process_getc() handle the
	 * refresh (because we might be "flat out" scrolling).
	 *
	 * If we read a small number of bytes on our last read(), then we should
	 * refresh the screen here, because if other tabs are busy, we can not
	 * count on rxvt_cmd_getc() refreshing the screen.
	 *
	 * XXX 2006-09-01 gi1242: If we have multiple visible terminal windows,
	 * then we should do this refresh for ALL tabs with visible terminal
	 * windows (and not only the active tab).
	 */

	if( AVTS(r)->nbytes_last_read <= r->h->refresh_limit )
	    rxvt_refresh_vtscr_if_needed( r );

	/*
	 * handle the case that some children have died regardless of what
	 * rxvt_cmd_getc returned
	 */
	if (r->ndead_childs > 0 || r->cleanDeadChilds )
	    rxvt_clean_cmd_page (r);

    } /* while(1) */
    /* NOT REACHED */
    assert (0);
}