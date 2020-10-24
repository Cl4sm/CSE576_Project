char *
strerror(num)
	int num;
{
	extern int sys_nerr;
	extern char *sys_errlist[];
#define	UPREFIX	"Unknown error: "
	static char ebuf[40] = UPREFIX;		/* 64-bit number + slop */
	register unsigned int errnum;
	register char *p, *t;
	char tmp[40];

	errnum = num;				/* convert to unsigned */
	if (errnum < sys_nerr)
		return(sys_errlist[errnum]);

	/* Do this by hand, so we don't include stdio(3). */
	t = tmp;
	do {
		*t++ = "0123456789"[errnum % 10];
	} while (errnum /= 10);
	for (p = ebuf + sizeof(UPREFIX) - 1;;) {
		*p++ = *--t;
		if (t <= tmp)
			break;
	}
	*p = '\0';
	return(ebuf);
}