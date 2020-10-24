void
da_unref(p)
	char *p;
{
	p -= PVMPAGEHDR;
#ifdef TEST_ADD
	if ((long) TEST_ADD(&((struct shmpghdr *)p)->pg_ref, -1L) < 0)
		pvmlogerror("ref count is negative!\n");
#else
	PAGELOCK(&((struct shmpghdr *)p)->pg_lock);
	if (--((struct shmpghdr *)p)->pg_ref < 0) {
		sprintf(pvmtxt, "ref = %d on page %d\n",
			((struct shmpghdr *)p)->pg_ref, (p - outmsgbuf)/pgsz + 1);
		pvmlogerror(pvmtxt);
	}
	PAGEUNLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
	if (((struct shmpghdr *)p)->pg_priv) {
		if (((struct shmpghdr *)p)->pg_ref < 1)
			PVM_FREE(p);
	} else {
		if (p >= outmsgbuf && p < outmsgbuf + outbufsz * pvmpgsz)
			nbufsowned--;
	}
}
