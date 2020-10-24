int
pvmputenv(s)
	char *s;	/* string of form "NAME=Value" */
{
	char *p;					/* gp */
	int l;						/* length of env var name or env */
	static char **myen = 0;		/* last environ that i created */
	char **newen;				/* new env if extending */
	char **ep, **ep2;			/* gp */

	/* sanity check the string and locate the '=' */

	if (!s)
		return -1;
	for (p = s; *p && *p != '='; p++);
	if (p == s || !*p)
		return -1;
	l = p - s + 1;	/* must include '=' */

	/* search current environ for name */

	for (ep = environ; *ep; ep++)
		if (!strncmp(*ep, s, l))
			break;

	if (*ep) {	/* already there, replace it */
		*ep = s;

	} else {	/* not there, must extend environ */

	/* length needed is? */

		for (l = 2, ep = environ; *ep; l++, ep++);

		if (environ == myen) {	/* mine, reallocate */
			if (!(newen = (char**)realloc((char *)environ,
					(unsigned)(l * sizeof(char*)))))
				return -1;

		} else {	/* not mine, alloc new and copy */
			if (!(newen = (char**)malloc((unsigned)(l * sizeof(char*)))))
				return -1;
			for (ep = environ, ep2 = newen; *ep2++ = *ep++; );
		}
		newen[l - 2] = s;
		newen[l - 1] = 0;
		environ = myen = newen;
	}
	return 0;
}
