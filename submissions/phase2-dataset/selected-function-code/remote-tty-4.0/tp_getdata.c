int
tp_getdata(int fd, ttyprot *tp) {
	int len = ntohs(tp->i);
	int nchars;

	if ((nchars = read(fd, tp->c, len)) != len) {
		dprintf(stderr, "tp_getdata: read=%d(%d) fd%d: ",
			nchars, len, fd);
		if (nchars < 0)
			perror("read#2");
		else
			fputc('\n', stderr);
		return (0);
	}
#ifdef DEBUG
	if (Debug >= 5) {
		fprintf(stderr, "tp_getdata(fd%d, len%d): got %d bytes",
			fd, len, nchars);
		if (Debug >= 6) {
			fputs(": \"", stderr);
			cat_v(stderr, tp->c, nchars);
			fputs("\"", stderr);
		}
		fputc('\n', stderr);
	}
#endif
	return (nchars);
}
