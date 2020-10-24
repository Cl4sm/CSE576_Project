char *
da_new(len)
	int len;
{
	char *p;
	int start;

	if (len > pvmpgsz - PVMPAGEHDR) {
		sprintf(pvmtxt, "da_new() len = %d: frag must fit in a page\n", len);
		pvmlogerror(pvmtxt);
		return 0;
	}

#ifdef IMA_CSPP
	p = outmsgbuf + nxtpage*pvmpgsz;
#else
	p = outmsgbuf;
	if (nxtpage && !((struct shmpghdr *)p)->pg_ref)
		nxtpage = 0;		/* recycle to avoid page fault */
	else
		p += nxtpage*pvmpgsz;
#endif

	start = nxtpage;
	while (1) {
		if (((struct shmpghdr *)p)->pg_ref == 0) {
			nbufsowned++;
			break;
		}
		if (++nxtpage == outbufsz) {
			nxtpage = 0;
			p = outmsgbuf;

		} else
			p += pvmpgsz;

		if (nxtpage == start) {		/* buffer full */
			p = TALLOC(len + PVMPAGEHDR, char, "data");
			if (p) {
				static int once = 1;

				if (once) {
					once = 0;
					pvmlogerror("da_new() Warning: shared buffer full, using malloc\n");
				}
#if !defined(IMA_KSR1) && !defined(TEST_ADD)
				PAGEINITLOCK(&((struct shmpghdr *)p)->pg_lock);
#endif
				((struct shmpghdr *)p)->pg_priv = 1;
			}
			break;
		}
	}

	if (nxtpage >= bufpageused)
		bufpageused = nxtpage + 1;
	if (p) {
		((struct shmpghdr *)p)->pg_ref = 1;
		return (p + PVMPAGEHDR);
	} else
		return 0;
}
