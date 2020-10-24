int
lpvmerr(f, n)
	char *f;		/* error location */
	int n;			/* error code */
{
	char buf[128];

	pvm_errno = n;
	if (pvmautoerr) {
		buf[0] = 0;
		strncat(buf, f, sizeof(buf)-4);
		strcat(buf, "()");
		pvm_perror(buf);
		fflush(stderr);
		if (pvmautoerr == 3)
			abort();
		if (pvmautoerr == 2) {
			pvmautoerr = 1;
			pvm_exit();
			exit(n);
		}
	}
	return n;
}
