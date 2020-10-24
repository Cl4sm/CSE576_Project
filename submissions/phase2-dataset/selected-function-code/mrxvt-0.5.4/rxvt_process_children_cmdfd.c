void
rxvt_process_children_cmdfd( rxvt_t* r, fd_set* p_readfds )
{
    /*
     * Handle the children that have generate input. Notice in this loop we only
     * process input, but do NOT determine the child we want to return.
     */
    register int    i;

    for (i = 0; i <= LTAB(r); i++)
    {
	unsigned int	count, bufsiz;

	/* check for activity */
        rxvt_monitor_tab(r,i);

	/* check next file descriptor if this one has nothing to read in. */
	if (!FD_ISSET(PVTS(r, i)->cmd_fd, p_readfds))
	{
	    PVTS(r, i)->nbytes_last_read = 0;
	    PVTS(r, i)->scrolled_lines	 = 0;
	    continue;
	}

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "reading from shell %d\n", i));

	/* check our command buffer before reading data */
	rxvt_check_cmdbuf( r, i );

	/* The buffer size is the buffer length - used length */
	count = bufsiz = (BUFSIZ - 1) -
	    (PVTS(r, i)->cmdbuf_endp - PVTS(r, i)->cmdbuf_base);

	/* read data from the command fd into buffer */
	count -= rxvt_read_child_cmdfd (r, i, count);

#if 0
	/* check if a child died */
	if( PVTS(r, i)->dead && errno == EIO )
	    *PVTS(r, i)->cmdbuf_endp = (char) 0;
#endif

	/* highlight inactive tab if there is some input */
	if(
	     NOTSET_OPTION(r, Opt2_hlTabOnBell)	    &&
	     bufsiz != count			    &&
	     i != ATAB(r)
	   )
	{
	    rxvt_tabbar_highlight_tab (r, i, False);
	}
    }   /* for loop */
}