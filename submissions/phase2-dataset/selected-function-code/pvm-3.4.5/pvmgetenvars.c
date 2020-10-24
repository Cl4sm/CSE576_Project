pvmgetenvars(ep)
	char ***ep;
{
	char **xpl;			/* vars to export */
	int mxpl;			/* cur length of xpl */
	int nxpl;			/* num vars found */
	char buf[200];
	char *p, *q;
	int n;

	if (p = getenv("PVM_EXPORT")) {
		mxpl = 5;
		xpl = TALLOC(mxpl, char *, "env");
		xpl[0] = p - 11;
		nxpl = 1;
		while (1) {
			while (*p == ':')
				p++;
			if (!*p)
				break;
			n = (q = CINDEX(p, ':')) ? q - p : strlen(p);
			strncpy(buf, p, n);
			buf[n] = 0;
			if (q = getenv(buf)) {
				if (nxpl == mxpl) {
					mxpl += mxpl / 2 + 1;
					xpl = TREALLOC(xpl, mxpl, char *);
				}
				xpl[nxpl++] = q - n - 1;
			}
			p += n;
		}
		*ep = xpl;
		return nxpl;

	} else {
		return 0;
	}
}
