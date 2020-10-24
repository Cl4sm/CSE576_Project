void
rxvt_process_getc( rxvt_t *r, int page, unsigned char ch )
{
    int		    limit;	/* Number of lines to read before asking for a
				   refresh */

    limit = r->h->skip_pages * r->TermWin.nrow;
    if( limit < 0 )
	/* Integer overflow */
	limit = INT_MAX;
    

    /*
     * Process as much input from the tab as is available. Keep a count of the
     * (approximate) number of lines we have scrolled, so we know when to
     * refresh.
     */
    for(;;)
    {
	if (ch >= ' ' || ch == '\t' || ch == '\n' || ch == '\r')
	{
	    /*
	     * Read the longest text string we can from the input buffer.
	     */

	    int	    nlines = 0,		/* #lines read */
		    nchars,		/* #chars read before newline */
		    refreshnow = 0;	/* If we should request a refresh */

	    unsigned char   *str;


	    nchars = PSCR(r, page).cur.col;

	    /*
	     * point `str' to the start of the string, decrement first since
	     * it was post incremented in rxvt_cmd_getc()
	     */
	    str = --(PVTS(r, page)->cmdbuf_ptr);
	    while (PVTS(r, page)->cmdbuf_ptr < PVTS(r, page)->cmdbuf_endp)
	    {
		ch = *(PVTS(r, page)->cmdbuf_ptr)++;
		
		if (ch == '\n')
		{
		    nchars = 0;
		    nlines++;
		    PVTS(r, page)->scrolled_lines++;
		}

		else if (ch < ' ' && ch != '\t' && ch != '\r')
		{
		    /*
		     * Unprintable. Reduce cmdbuf_ptr so that this character
		     * will be processed later.
		     */
		    PVTS(r, page)->cmdbuf_ptr--;
		    break;
		}

		else if( ++nchars > r->TermWin.ncol )
		{
		    PVTS(r, page)->scrolled_lines++;
		    nchars = 0;
		}

		if(
		     PVTS(r, page)->mapped			&&
		     PVTS(r, page)->scrolled_lines >= limit
		  )
		{
		    refreshnow = 1;
		    break;
		}
	    }

	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "\e[31mAdding %d chars %d lines in tab %d\e[0m\n%.*s\n", PVTS(r, page)->cmdbuf_ptr - str, nlines, page, PVTS(r, page)->cmdbuf_ptr - str, str));

	    /*
	     * NOTE: nlines can not be MORE than the number of lines we will
	     * actually add!
	     */
	    rxvt_scr_add_lines(r, page, str, nlines,
		(PVTS(r, page)->cmdbuf_ptr - str));

	    /*
	     * Only refresh the screen if we've scrolled more than
	     * MAX_SKIPPED_PAGES pages.
	     *
	     * Refreshing should be correct for small scrolls, because
	     * nbytes_last_read will be small, forcing the refresh.
	     */
	    if (refreshnow)
	    {
		refreshnow = 0;

		/*
		 * Note: If the tab is not visible, then rxvt_scr_refresh
		 * returns immediately. Also rxvt_scr_refresh resets
		 * scrolled_lines.
		 */
		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Requesting refresh." " Active tab (%d) scrolled %d lines\n", ATAB(r), AVTS(r)->scrolled_lines ));
		rxvt_scr_refresh(r, page,
			(r->h->refresh_type & ~CLIPPED_REFRESH) );

		/* If we have X events to process, then do so now. */
		if( XPending( r->Xdisplay ) )
		    break;
	    }
	}

	/*
	 * Process escape sequence
	 */
	else if( ch == C0_ESC )
	{
	    /* Save the start of the escape sequence */
	    if( IS_NULL( PVTS(r, page)->cmdbuf_escstart ) )
		PVTS(r, page)->cmdbuf_escstart =
		    PVTS(r, page)->cmdbuf_ptr-1;

	    /* Forget the previous escape sequence failure (if any) */
	    SET_NULL( PVTS(r, page)->cmdbuf_escfail );

	    /* Attempt to process the escape sequence */
	    rxvt_process_escape_seq(r, page);

	    /* If we succeeded, then clear the start. */
	    if( IS_NULL( PVTS(r, page)->cmdbuf_escfail ) )
		SET_NULL( PVTS(r, page)->cmdbuf_escstart );
	    else
		/* Otherwise don't process any more data from this tab */
		break;
	}

	/*
	 * Anything else must be a non-printing character
	 */
	else
	{
	    rxvt_process_nonprinting(r, page, ch);
	}


	/*
	 * Check if we can keep reading on this tab.
	 *
	 * NOTE: We could check if we also have pending X events, but this will
	 * generate many many extra protocol requests, which can be quite a
	 * problem on a slow connection. Thus for now we only process X events
	 * on screen refreshes or in rxvt_cmd_getc().
	 */
	if( rxvt_cmdbuf_has_input( r, page ) )
	    ch = *PVTS(r,page)->cmdbuf_ptr++;
	else
	    break;
    } /* for(;;) */
}