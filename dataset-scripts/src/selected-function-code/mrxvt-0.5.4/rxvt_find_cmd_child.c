int
rxvt_find_cmd_child (rxvt_t* r)
{
    register int    k;
    static int	    lastProcessed = 0;  /* tab we processed last time */

    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_find_cmd_child()\n" ));

    /*
     * See if the active tab has input before anything else.
     */
    if( rxvt_cmdbuf_has_input( r, ATAB(r) ) )
	return ATAB(r);

    /*
     * Now look for data from other tabs. Remember the tab we found data from so
     * that we can start from the next tab on the next call to this function.
     */
    if( lastProcessed > LTAB(r) )   /* Sanity check */
	lastProcessed = LTAB(r);

    /* start from the next tab of last processed tab */
    k = lastProcessed + 1;

    do
      {
	if( k > LTAB(r) )	/* round-robin */
	    k = 0;

	assert( PVTS(r, k)->cmdbuf_base <= PVTS(r, k)->cmdbuf_endp );

	/* already have something in some page's buffer */
	if( rxvt_cmdbuf_has_input(r, k) )
	{
	    lastProcessed = k;
	    return k;
	}

      }
    while (k++ != lastProcessed);	/* until we hit the last child again */

    return -1; /* not found */
}