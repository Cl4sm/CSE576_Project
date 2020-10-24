tt_write(const unsigned char *d, int len)
{
    int             riten, p;

    if (v_bufstr == NULL && len > 0) {
	v_buffer = v_bufstr = v_bufptr = MALLOC(len);
	v_bufend = v_buffer + len;
    }
/*
 * Append to the block we already have.  Always doing this simplifies the
 * code, and isn't too bad, either.  If this is a short block, it isn't
 * too expensive, and if this is a long block, we won't be able to write
 * it all anyway.
 */
    if (len > 0) {
	if (v_bufend < v_bufptr + len) {	/* we've run out of room */
	    if (v_bufstr != v_buffer) {
	    /* there is unused space, move everything down */
	    /* possibly overlapping bcopy here */
	    /* bcopy(v_bufstr, v_buffer, v_bufptr - v_bufstr); */
		memcpy(v_buffer, v_bufstr, v_bufptr - v_bufstr);
		v_bufptr -= v_bufstr - v_buffer;
		v_bufstr = v_buffer;
	    }
	    if (v_bufend < v_bufptr + len) {
	    /* still won't fit: get more space */
	    /* Don't use XtRealloc because an error is not fatal. */
		int             size = v_bufptr - v_buffer;

	    /* save across realloc */
		v_buffer = REALLOC(v_buffer, size + len);
		if (v_buffer) {
		    v_bufstr = v_buffer;
		    v_bufptr = v_buffer + size;
		    v_bufend = v_bufptr + len;
		} else {
		/* no memory: ignore entire write request */
		    print_error("cannot allocate buffer space");
		    v_buffer = v_bufstr;	/* restore clobbered pointer */
		}
	    }
	}
	if (v_bufend >= v_bufptr + len) {	/* new stuff will fit */
	    memcpy(v_bufptr, d, len);	/* bcopy(d, v_bufptr, len); */
	    v_bufptr += len;
	}
    }
/*
 * Write out as much of the buffer as we can.
 * Be careful not to overflow the pty's input silo.
 * We are conservative here and only write a small amount at a time.
 *
 * If we can't push all the data into the pty yet, we expect write
 * to return a non-negative number less than the length requested
 * (if some data written) or -1 and set errno to EAGAIN,
 * EWOULDBLOCK, or EINTR (if no data written).
 *
 * (Not all systems do this, sigh, so the code is actually
 * a little more forgiving.)
 */

#define MAX_PTY_WRITE 128	/* 1/2 POSIX minimum MAX_INPUT */

    if ((p = v_bufptr - v_bufstr) > 0) {
	riten = write(cmd_fd, v_bufstr, p < MAX_PTY_WRITE ? p : MAX_PTY_WRITE);
	if (riten < 0)
	    riten = 0;
	v_bufstr += riten;
	if (v_bufstr >= v_bufptr)	/* we wrote it all */
	    v_bufstr = v_bufptr = v_buffer;
    }
/*
 * If we have lots of unused memory allocated, return it
 */
    if (v_bufend - v_bufptr > 1024) {	/* arbitrary hysteresis */
    /* save pointers across realloc */
	int             start = v_bufstr - v_buffer;
	int             size = v_bufptr - v_buffer;
	int             allocsize = size ? size : 1;

	v_buffer = REALLOC(v_buffer, allocsize);
	if (v_buffer) {
	    v_bufstr = v_buffer + start;
	    v_bufptr = v_buffer + size;
	    v_bufend = v_buffer + allocsize;
	} else {
	/* should we print a warning if couldn't return memory? */
	    v_buffer = v_bufstr - start;	/* restore clobbered pointer */
	}
    }
}
