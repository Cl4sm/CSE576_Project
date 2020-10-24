void read_from_socket(fd, pl, buf, buf_len, buf_size)
     int	fd;
     struct pipeline	*pl;
     char	*buf;
     int	*buf_len, buf_size;
{
    int	desired_read = buf_size - *buf_len;
    int	rval;

    if (pl && desired_read > pl->bytes_left) {
	desired_read = pl->bytes_left;
    }
#if DEBUG>1
    fprintf(stderr, "%s: read(sock_fd, buf+%d, %d)",
	    progname, *buf_len, desired_read);
#endif
    rval = read(fd, buf+*buf_len, desired_read);
#if DEBUG>1
    fprintf(stderr, "=%d;\n", rval);
#endif
    if (rval==0) {
	fprintf(stderr, "%s: encapsulation protocol error reading socket.  Socket closed prematurely by remote end.\n", progname);
	/*sock_closed = 1;*/
	exit(EXITCODE_PROTOCOL);
    } else if (rval<0) {
	if (errno!=EINTR) {
	    fprintf(stderr, "%s: error during read(sock_fd,,%d).  Aborting.  ",
		    progname, desired_read);
	    perror("");
	    exit(EXITCODE_SYSCALLFAILED);
	}
	/* never mind. try again later */
    } else {
	*buf_len += rval;

	if (pl) pl->bytes_left -= rval;
    }

    if (pl == &pl_bit_bucket) {
	/* throw away the bytes */
#ifdef DEBUG
	fprintf(stderr, "%s: %d bytes into the bit bucket\n", progname, *buf_len);
#endif
	*buf_len = 0;
    }
}