int
rxvt_read_child_cmdfd (rxvt_t* r, int page, unsigned int count)
{
    int		    n = 0, bread = 0;
    struct	    timeval  tp;

    while( count )
    {
	int readErrno;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "read maximal %u bytes\n", count));

	/*
	 * 2006-08-23 gi1242: O_NDELAY is set here, so we need not worry about
	 * calls to read() blocking.
	 */
	errno = PVTS(r, page)->gotEIO = 0;
	n = read( PVTS(r, page)->cmd_fd, PVTS(r, page)->cmdbuf_endp, count );
	readErrno = errno;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "read %d bytes\n", n));

	if (n > 0)
	{
	    /* Update count and buffer pointer */
	    count -= n;
	    bread += n;
	    PVTS(r, page)->cmdbuf_endp += n;
	}

	else if (0 == n)
	{
	    /* rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "Should not happen?\n")); */
	    /* 2006-08-23 gi1242: Could happen if we have no more data. */
	    break;
	}

	else /* if (n < 0) */
	{
	    /*
	     * We do not update count and buffer pointer and continue
	     * trying read more data in the next loop iteration.
	     */
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "%s\n", strerror(readErrno)));

	    assert( readErrno != EBADF && readErrno != EFAULT &&
		    readErrno != EISDIR );

	    /* See if this process is dead */
	    switch (readErrno)
	    {
		case EIO:
		    r->gotEIO = PVTS(r, page)->gotEIO = 1;
		case EINTR:
		    rxvt_mark_dead_childs(r);
		    break;
	    }

	    /*
	     * 2006-08-31 gi1242: Old code would only break out on EAGAIN or
	     * EINVAL.
	     */
	    break;

	}
    }	/* while (count) */

    if (bread != 0)
    {
	gettimeofday( &tp, NULL);
	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,
		    "output produced on epoch %i\n", tp.tv_sec));
    }
    PVTS(r, page)->monitor_nbytes_read += bread;
    PVTS(r, page)->nbytes_last_read = bread;
    return bread;
}