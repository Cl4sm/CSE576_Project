tp_senddata(int fd, const u_char *buf, int len, int typ) {
	struct iovec iov[2];
	ttyprot t;
	int n = 0;

#if DEBUG
	if (Debug >= 5) {
		fprintf(stderr, "tp_senddata(fd=%d, buf=\"", fd);
		cat_v(stderr, buf, len);
		fprintf(stderr, "\", len=%d, typ=%d)\r\n", len, typ);
	}
#endif
	t.f = htons(typ);
	iov[0].iov_base = (caddr_t)&t;
	iov[0].iov_len = TP_FIXED;
	while (len > 0) {
		int i = min(len, TP_MAXVAR);

		t.i = htons(i);
		iov[1].iov_base = (caddr_t)buf;
		iov[1].iov_len = i;
		buf += i;
		len -= i;
		i = writev(fd, iov, 2);
		if (i < 0)
			break;
		n += i;
	}
	return (n);
}
