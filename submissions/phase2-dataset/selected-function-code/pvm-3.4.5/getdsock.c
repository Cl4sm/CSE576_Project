void
getdsock()
{
	char buf[128];
	int d;
	int n;
	char *p;

	if (!(p = getenv("PVMSOCK"))) {
		if (!(p = pvmdsockfile())) {
			pvmlogerror("getdsock() pvmdsockfile() failed\n");
			return;
		}
		if ((d = open(p, O_RDONLY, 0)) == -1) {
			pvmlogperror(p);
			return;
		}
		n = read(d, buf, sizeof(buf));
		(void)close(d);
		if (n == -1) {
			pvmlogperror("getdsock() read addr file");
			return;
		}
		if (n == 0) {
			pvmlogerror("getdsock() read addr file: wrong length read\n");
			return;
		}
		buf[n] = 0;
		p = buf;
	}

	hex_inadport(p, &pvmdsad);
	pvmdsad.sin_family = AF_INET;
}
