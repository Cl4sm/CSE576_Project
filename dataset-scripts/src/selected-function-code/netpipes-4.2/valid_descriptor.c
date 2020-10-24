int valid_descriptor(int fd)
{
	int	rval;
	fd_set	fds;
	struct timeval	tv;

	tv.tv_sec = 0; tv.tv_usec = 0; /* POLL */

	FD_ZERO(&fds);
	FD_SET(fd, &fds);

	rval = select(fd+1, &fds, (fd_set *)0, (fd_set *)0, &tv);

	if (rval<0 && errno == EBADF) {
#ifdef DEBUG
	    fprintf(stderr, "%s: descriptor %d not in use\n",
		    progname, fd);
#endif
	    return 0;
	} else {
#ifdef DEBUG
	    fprintf(stderr, "%s: descriptor %d already in use\n",
		    progname, fd);
#endif
	    return 1;
	}
}