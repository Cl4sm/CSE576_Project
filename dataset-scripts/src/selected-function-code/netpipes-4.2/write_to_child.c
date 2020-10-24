int write_to_child(pl, buf, buf_len)
     struct pipeline	*pl;
     char	*buf;
     int	*buf_len;
{
    int	rval;

#if DEBUG>1
    fprintf(stderr, "%s: write(out[%d/c%d], buf, %d)",
	    progname, pl->pipe[0], pl->child_fd, *buf_len);
#endif
    rval = write(pl->pipe[0], buf, *buf_len);
#if DEBUG>1
    fprintf(stderr, "=%d;\n", rval);
#endif
    if (rval == 0) {
#ifdef DEBUG
	fprintf(stderr, "%s: Inexplicable!  write(out[%d],,%d) returns 0", progname, pl->pipe[0], *buf_len);
#endif
    } else if (rval<0) {
	if (errno == EINTR) {
	    return 0;		/* no biggie, retry later */
	} else if (errno == EPIPE) {
	    /* DOH! fake it */
	    fprintf(stderr, "%s: EPIPE while writing to child fd %d\n", progname, pl->child_fd);
	    exitcode_warnings = EXITCODE_GOT_RESET;
	    pl->specact = CLOSE_TO_READ;
	    *buf_len = 0;
	    send_reset(pl);
	    return 0;		/* this is NOT a normal thing */
	} else {
	    fprintf(stderr, "%s: error during write(out[%d],,).  Aborting.  ",
		    progname, pl->pipe[0]);
	    perror("");
	    exit(EXITCODE_SYSCALLFAILED);
	}
    } else {
	memmove(buf, buf+rval, *buf_len - rval);
	*buf_len -= rval;
    }
    return rval;
}