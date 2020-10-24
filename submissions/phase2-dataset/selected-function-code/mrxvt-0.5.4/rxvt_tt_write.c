void
rxvt_tt_write(rxvt_t* r, int page, const unsigned char *d, int len)
{
#define MAX_PTY_WRITE 128   /* 1/2 POSIX minimum MAX_INPUT */
    register int    k, beg, end;

    if (ISSET_OPTION(r, Opt2_broadcast))
    {
	beg = 0; end = LTAB(r);
    }
    else
    {
	beg = end = page;
    }

    for (k = beg; k <= end; k ++)
    {
	int		riten;
	int		p;
	/* start of physical buffer	*/
	unsigned char*	v_buffer;
	/* start of current buffer pending */
	unsigned char*	v_bufstr;
	/* end of current buffer pending   */
	unsigned char*	v_bufptr;
	/* end of physical buffer	  */
	unsigned char*	v_bufend;

	rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "rxvt_tt_write %d (%s)\n", k, d ? (char*) d: "nil"));

	if (IS_NULL(PVTS(r, k)->v_bufstr) && len > 0)
	{
	    p = (len / MAX_PTY_WRITE + 1) * MAX_PTY_WRITE;
	    if (p <= 0) /* possible integer overflow */
		return ;
	    v_buffer = v_bufstr = v_bufptr = rxvt_malloc(p);
	    v_bufend = v_buffer + p;
	}
	else
	{
	    v_buffer = PVTS(r, k)->v_buffer;
	    v_bufstr = PVTS(r, k)->v_bufstr;
	    v_bufptr = PVTS(r, k)->v_bufptr;
	    v_bufend = PVTS(r, k)->v_bufend;
	}

	/*
	 * Append to the block we already have. Always doing this simplifies the
	 * code, and isn't too bad, either. If this is a short block, it isn't
	 * too expensive, and if this is a long block, we won't be able to write
	 * it all anyway.
	 */
	if (len > 0)
	{
	    if (v_bufend < v_bufptr + len)
	    {
		/* run out of room */
		if (v_bufstr != v_buffer)
		{
		    /* there is unused space, move everything down */
		    MEMMOVE(v_buffer, v_bufstr,
			(unsigned int)(v_bufptr - v_bufstr));
		    v_bufptr -= v_bufstr - v_buffer;
		    v_bufstr = v_buffer;
		}
		if (v_bufend < v_bufptr + len)
		{
		    /* still won't fit: get more space, use most basic
		    ** realloc because an error is not fatal. */
		    unsigned int    size = v_bufptr - v_buffer;
		    unsigned int    reallocto;

		    reallocto = ((size+len) / MAX_PTY_WRITE + 1) *MAX_PTY_WRITE;
		    v_buffer = rxvt_realloc(v_buffer, reallocto);
		    /* save across realloc */
		    if (v_buffer)
		    {
			v_bufstr = v_buffer;
			v_bufptr = v_buffer + size;
			v_bufend = v_buffer + reallocto;
		    }
		    else
		    {
			/* no memory: ignore entire write request */
			rxvt_msg (DBG_ERROR, DBG_COMMAND, 
				"data loss: cannot allocate buffer space");
			/* restore clobbered pointer */
			v_buffer = v_bufstr;
		    }
		}
	    }
	    if (v_bufend >= v_bufptr + len)
	    {
		/* new stuff will fit */
		MEMCPY(v_bufptr, d, len);
		v_bufptr += len;
	    }
	}

	/*
	 * Write out as much of the buffer as we can. Be careful not to overflow
	 * the pty's input silo. We are conservative here and only write a small
	 * amount at a time.
	 *
	 * If we can't push all the data into the pty yet, we expect write to
	 * return a non-negative number less than the length requested (if some
	 * data written) or -1 and set errno to EAGAIN, EWOULDBLOCK, or EINTR
	 * (if no data written).
	 *
	 * (Not all systems do this, sigh, so the code is actually a little more
	 * forgiving.)
	 */

	if ((p = v_bufptr - v_bufstr) > 0)
	{
	    riten = write(PVTS(r, k)->cmd_fd, v_bufstr, min(p, MAX_PTY_WRITE));
	    rxvt_dbgmsg ((DBG_DEBUG, DBG_COMMAND,  "tt_write %d chars to vts[%d].cmd_fd = %d\n", riten, k, PVTS(r, k)->cmd_fd));
	    if (riten < 0)
		riten = 0;
	    v_bufstr += riten;
	    if (v_bufstr >= v_bufptr)	/* we wrote it all */
		v_bufstr = v_bufptr = v_buffer;
	}

	/*
	 * If we have lots of unused memory allocated, return it
	 */
	if (v_bufend - v_bufptr > MAX_PTY_WRITE * 8)
	{
	    /* arbitrary hysteresis, save pointers across realloc */
	    unsigned int    start = v_bufstr - v_buffer;
	    unsigned int    size = v_bufptr - v_buffer;
	    unsigned int    reallocto;

	    reallocto = (size / MAX_PTY_WRITE + 1) * MAX_PTY_WRITE;
	    v_buffer = rxvt_realloc(v_buffer, reallocto);
	    if (v_buffer)
	    {
		v_bufstr = v_buffer + start;
		v_bufptr = v_buffer + size;
		v_bufend = v_buffer + reallocto;
	    }
	    else
	    {
		/* should we print a warning if couldn't return memory? */
		/* restore clobbered pointer */
		v_buffer = v_bufstr - start;
	    }
	}
	PVTS(r, k)->v_buffer = v_buffer;
	PVTS(r, k)->v_bufstr = v_bufstr;
	PVTS(r, k)->v_bufptr = v_bufptr;
	PVTS(r, k)->v_bufend = v_bufend;
    }	/* for */
}