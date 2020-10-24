	char *cp;		/* bytes */
	int n;			/* length */
	char *pad;		/* tag string */
{
	static char *buf = 0;
	static int bufl = 0;

	int i, l;
	char *r;

	pad = pad ? pad : "";
	l = strlen(pad) + 50;
	if (l > bufl) {
		if (buf)
			PVM_FREE(buf);
		if (!(buf = TALLOC(l, char, "hdump"))) {
			bufl = 0;
			pvmlogerror("pvmhdump() malloc failed\n");
			return 1;
		}
		bufl = l;
	}

	r = buf;
	for (i = 0; n-- > 0; i = (i + 1) & 15) {
		sprintf(r, "%s%02x",
			i ? " " : pad,
			0xff & *cp++);
		r += strlen(r);
		if (!n || i == 15) {
			strcpy(r, "\n");
			pvmlogerror(buf);
			r = buf;
		}
	}

	return 0;
}
