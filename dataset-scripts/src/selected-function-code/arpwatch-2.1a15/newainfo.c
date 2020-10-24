struct ainfo *
newainfo(void)
{
	register struct ainfo *ap;
	register u_int size;
	static struct ainfo *ainfoptr = NULL;
	static u_int ainfosize = 0;

	if (ainfosize == 0) {
		ainfosize = 512;
		size = ainfosize * sizeof(*ap);
		ap = (struct ainfo *)malloc(size);
		if (ap == NULL) {
			syslog(LOG_ERR, "newainfo(): malloc: %m");
			exit(1);
		}
		memset((char *)ap, 0, size);
		ainfoptr = ap;
	}
	ap = ainfoptr++;
	--ainfosize;
	return (ap);
}