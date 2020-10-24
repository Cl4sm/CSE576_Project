pvmenvinsert(anep, s)
	char ***anep;		/* environment pointer, may change (NOT environ) */
	char *s;			/* string of form "NAME=Value" */
{
	char **ep;
	char **newep;
	char *p;
	int l;						/* length of env var name or env */

	newep = *anep;

	if (!s || !(p = CINDEX(s, '=')))
		return -1;
	l = p - s + 1;			/* must include the '=' */

	/* search environ for name */

	for (ep = newep; *ep; ep++)
		if (!strncmp(*ep, s, l))
			break;

	if (*ep) {				/* already there, replace it */
		PVM_FREE(*ep);
		*ep = STRALLOC(s);

	} else {				/* not there, must extend environ */

	/* length needed is? */

		for (l = 2, ep = newep; *ep; l++, ep++);

		if (!(newep = TREALLOC((char *)newep, l, char *)))
			return -1;

		newep[l - 2] = STRALLOC(s);
		newep[l - 1] = 0;
		*anep = newep;
	}
	return 0;
}
