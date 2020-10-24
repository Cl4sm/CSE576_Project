void
rxvt_clean_cmd_page (rxvt_t* r)
{
    int		i;

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_clean_cmd_page()\n" ));
    if( r->ndead_childs )
	rxvt_mark_dead_childs( r );

   /*
    * We had better not get here unless we need to clean up dead children. Make
    * sure we don't proceed when we receive spurious dead child messages (e.g.
    * from when the print pipe dies).
    */
    if( !r->cleanDeadChilds )
	return;

    /*
     * We start from the last child because we need to move ahead after
     * removing dead child. This makes it much safer.
     *
     * Why do we need to restart dead value from LTAB(r) again? Because a
     * child may have died when we do something following and changed the
     * value of r->ndead_childs! This child may be later than any dead children
     * we have examined.
     */
    for (i = LTAB(r); i >= 0; i--)
    {
	if( PVTS(r, i)->dead && PVTS(r, i)->hold == 1 )
	{
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, 
			"Tab %d exit %s (status %d). holdOption: %d\n",
			i, WIFEXITED(PVTS(r,i)->status) ? "success" : "failure",
			PVTS(r,i)->status,
			PVTS(r,i)->holdOption));
	    /*
	     * Process in tab i has died, and needs to be cleaned up.
	     */
	    if( SHOULD_HOLD( r, i ) )
	    {
		const int	maxLen = 1024;
		const char	*msg;

		rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
			    "Hold child %d after it died\n", i));

		/* increase hold number, so next iteration will skip it */
		PVTS(r, i)->hold++;

		/*
		 * Process any pending data from the child.
		 */
		do
		  {
		    unsigned char *last_escfail = NULL;

		    /*
		     * Process information in the child's output buffer.
		     */
		    while( PVTS(r, i)->cmdbuf_ptr < PVTS(r, i)->cmdbuf_endp )
		    {
			rxvt_process_getc( r, i, *(PVTS(r,i)->cmdbuf_ptr++) );

			/* Incomplete escape sequence. */
			if( PVTS(r, i)->cmdbuf_escfail )
			{
			    /*
			     * See if reading from the child's fd will complete
			     * this escape seqeunce.
			     */
			    if( IS_NULL( last_escfail ) )
				last_escfail = PVTS(r, i)->cmdbuf_escfail;

			    else
			    {
				/* Really incomplete escape sequence */
				rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND, "Incomplete escape sequence '%.*s'\n", 
					      PVTS(r, i)->cmdbuf_escfail - PVTS(r, i)->cmdbuf_escstart + 1, 
					      PVTS(r, i)->cmdbuf_escstart+1));

				SET_NULL( last_escfail );
				SET_NULL( PVTS(r, i)->cmdbuf_escstart );
				SET_NULL( PVTS(r, i)->cmdbuf_escfail );

				/* Skip the escape char */
				PVTS(r, i)->cmdbuf_ptr++;
			    }
			} /* if( PVTS(r, i)->cmdbuf_escfail ) */
		    }

		    /*
		     * Write out pending data in the child's input buffer.
		     */
		    if (PVTS(r, i)->v_bufstr < PVTS(r, i)->v_bufptr)
			rxvt_cmd_write(r, i, NULL, 0);

		    /* Make place for new data */
		    rxvt_check_cmdbuf( r, i );

		    /* Read any remaining data from childs fd */
		    rxvt_read_child_cmdfd( r, i,
			    BUFSIZ - 1 -
				(PVTS(r, i)->cmdbuf_endp
					- PVTS(r, i)->cmdbuf_base) );
		  }
		while( rxvt_cmdbuf_has_input( r, i) );

		/*
		 * print holdExitText on screen if defined.
		 */
		msg = getProfileOption( r, PVTS(r,i)->profileNum,
				Rs_holdExitTxt );
		if( NOT_NULL( msg ) && *msg )
		{
		    unsigned char   buffer[maxLen];
		    int		    len;

		    rxvt_percent_interpolate( r, i, msg, STRLEN(msg),
			    (char*) buffer, maxLen );

		    len = rxvt_str_escaped( (char*) buffer );

		    rxvt_cmd_write(r, i, buffer, len );

		    if( PVTS(r, i)->cmdbuf_ptr < PVTS(r, i)->cmdbuf_endp )
			rxvt_process_getc( r, i, *(PVTS(r,i)->cmdbuf_ptr++) );
		}

		/*
		 * Update title to show tab has finished.
		 */
		msg = getProfileOption( r, PVTS(r,i)->profileNum,
				Rs_holdExitTtl );
		if( NOT_NULL( msg ) && *msg )
		{
		    unsigned char    tabTitle[maxLen];

		    rxvt_percent_interpolate( r, i, msg, STRLEN(msg),
			    (char*) tabTitle, maxLen );
		    rxvt_str_escaped( (char*) tabTitle );

		    rxvt_tabbar_set_title( r, i, tabTitle );
		}
	    } /* if( SHOULD_HOLD( r, i ) ) */
	    else
		rxvt_remove_page( r, i );
	}
    } /* for(i) */

    r->cleanDeadChilds = 0; /* Dead child cleanup complete. */
}