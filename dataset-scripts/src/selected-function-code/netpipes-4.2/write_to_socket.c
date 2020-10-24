void write_to_socket(fd, obuf, obuf_off, obuf_len)
     int	fd;
     char	*obuf;
     int	*obuf_off;
     int	obuf_len;
{
    int	rval;

#if DEBUG>1
    fprintf(stderr, "%s: write(sock_fd, buf+%d, %d-%d)",
	    progname, *obuf_off, obuf_len, *obuf_off);
#endif
    rval = write(fd, obuf+ *obuf_off, obuf_len-*obuf_off);
#if DEBUG>1
    fprintf(stderr, "=%d;\n", rval);
#endif
    if (rval==0) {
	fprintf(stderr, "%s: Inexplicable!  write(sock_fd,,%d) returns 0\n",
		progname, obuf_len-*obuf_off);
	exit(EXITCODE_SYSCALLFAILED);
    } else if (rval<0) {
	if (errno!=EINTR) {
	    fprintf(stderr, "%s: error during write(sock_fd,,%d).  Aborting.  ",
		    progname, obuf_len-*obuf_off);
	    perror("");
	    exit(EXITCODE_SYSCALLFAILED);
	}
	/* never mind. try again later */
    } else {
	*obuf_off += rval;
    }
}