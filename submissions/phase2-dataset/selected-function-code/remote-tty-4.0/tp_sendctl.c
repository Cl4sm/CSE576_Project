tp_sendctl(int fd, u_int f, u_int i, u_char *c) {
	struct iovec iov[2];
	ttyprot t;
	int len = c ?min(strlen((char *)c), TP_MAXVAR) :0;
	int il = 0;

#if DEBUG
	if (Debug >= 5) {
		fprintf(stderr, "tp_sendctl(fd=%d, f=%04x, i=%d, c=\"",
			fd, f, i);
		cat_v(stderr, c ?c :(u_char*)"", len);
		fprintf(stderr, "\")\r\n");
	}
#endif
	t.f = htons(f);
	t.i = htons(i);
	iov[il].iov_base = (caddr_t)&t;
	iov[il].iov_len = TP_FIXED;
	il++;
	if (c) {
		iov[il].iov_base = (caddr_t)c;
		iov[il].iov_len = len;
		il++;
	}
	return (writev(fd, iov, il));
}
