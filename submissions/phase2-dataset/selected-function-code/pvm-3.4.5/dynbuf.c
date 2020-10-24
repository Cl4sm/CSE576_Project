dynbuf(tid, len)
	int tid;
	int len;
{
	struct peer *pp;
	int fd;
	char fname[32];
	struct shmpghdr *ph;
	char *pvmtmp;

	while (!(pp = peer_conn(tid, (int *)0)))
		;
/* XXX this doesn't check for peer_conn returning -1 */
	if (len > SHMBUFSIZE && len > pp->p_dlen && pp->p_dbuf) {
		munmap((caddr_t)pp->p_dbuf, SHMBUFSIZE);
		pp->p_dbuf = 0;
	}

	if (!(ph = (struct shmpghdr *)pp->p_dbuf)) {
		pvmtmp = pvmgettmp();
		sprintf(fname, PVMSHMFILE, pvmtmp, tid);
		if ((fd = open(fname, O_CREAT|O_RDWR, 0600)) == -1 ||
		(pp->p_dbuf = (char *)mmap(0, max(len,SHMBUFSIZE), PROT_READ|PROT_WRITE,
#if defined(IMA_SGIMP) || defined(IMA_SGIMP6) || defined(IMA_SGIMP64)
		MAP_SHARED|MAP_AUTOGROW, fd, 0)) == (char *)-1L)
#else
		MAP_SHARED, fd, 0)) == (char *)-1)
#endif
		{
			pvmlogperror(fname);
			return (char *)-1L;
		}
#ifdef IMA_SUNMP
		/* fill buffer with 0's */
		lseek(fd, len - 1, SEEK_SET);
		write(fd, fname, 1);
#endif
		close(fd);
		pp->p_dlen = len;
	} else if (ph->pg_ref) {	/* previous msg has not been recv'd */
		while (ph->pg_ref)
			if (peer_wait() == -1)
				return (char *)-1L;
#ifdef IMA_SUNMP
	} else if (pp->p_dlen < len) {
		if ((fd = open(fname, O_CREAT|O_RDWR, 0600)) == -1) {
			pvmlogperror(fname);
			return (char *)-1L;
		}
		lseek(fd, len - 1, SEEK_SET);
		write(fd, fname, 1);
		close(fd);
		pp->p_dlen = len;
#endif
	}

	return pp->p_dbuf;
}
