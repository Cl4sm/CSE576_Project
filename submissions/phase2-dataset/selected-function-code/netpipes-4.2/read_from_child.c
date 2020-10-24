int read_from_child(pl, buf, buf_len, buf_size)
     struct pipeline	*pl;
     char	*buf;
     int	*buf_len;
     int	buf_size;
{
    int	rval;

#if DEBUG>1
    fprintf(stderr, "%s: read(in[c%d], buf+%d, %d-%d)",
	    progname, pl->child_fd, *buf_len, buf_size, *buf_len);
#endif
    rval = read(pl->pipe[0], buf+*buf_len, buf_size - *buf_len);
#if DEBUG>1
    fprintf(stderr, "=%d;\n", rval);
#endif
    if (rval == 0) {
#ifdef DEBUG
	fprintf(stderr, "%s: closing in[%d=c%d]\n", progname, pl->pipe[0], pl->child_fd);
#endif
	pl->specact = CLOSE_TO_WRITE;
    } else if (rval<0) {
	if (errno == EINTR) {
	    return 0;		/* no biggie, retry later */
	} else {
	    fprintf(stderr, "%s: error during read(in[%d],,).  Aborting.  ",
		    progname, pl->pipe[0]);
	    perror("");
	    exit(EXITCODE_SYSCALLFAILED);
	}
    } else {
	*buf_len += rval;
    }
    return rval;
}