extern int
remove_feature(char **f, char *o)
{
	int c = 0, d, l;
	char *e, *p, *n;

	if (!f || !*f)
		return 1;

	/* Nothing to do */
	if (!o || *o == '\0')
		return 0;

	/* Check if not present */
	if (!strstr(*f, o))
		return 0;

	/* Get feature count */
	c = strtoul(*f, &e, 10);
	if (*f == e)
		/* parse error */
		return 1;

	/* Normalize features */
	while (*o == ' ') {
		o++;
	}
	/* Just spaces, return */
	if (*o == '\0')
		return 0;
	e = o + strlen(o);
	while (*e == ' ')
		e--;
	d = (int)(e - o);

	/* Update feature count */
	c--;
	p = o;
	while (p[0] != '\0') {
		if (p[0] == ' ' && p[1] != ' ' && p[1] != '\0')
			c--;
		p++;
	}

	/* Quick exit if all features have been removed */
	if (c == 0) {
		n = MALLOC(2);
		if (!n)
			return 1;
		strcpy(n, "0");
		goto out;
	}

	/* Search feature to be removed */
	e = strstr(*f, o);
	if (!e)
		/* Not found, return */
		return 0;

	/* Update feature count space */
	l = strlen(*f) - d;
	n =  MALLOC(l + 1);
	if (!n)
		return 1;

	/* Copy the feature count */
	sprintf(n, "%0d", c);
	/*
	 * Copy existing features up to the feature
	 * about to be removed
	 */
	p = strchr(*f, ' ');
	if (!p)
		/* Internal error, feature string inconsistent */
		return 1;
	while (*p == ' ')
		p++;
	p--;
	if (e != p) {
		do {
			e--;
			d++;
		} while (*e == ' ');
		e++; d--;
		strncat(n, p, (size_t)(e - p));
		p += (size_t)(e - p);
	}
	/* Skip feature to be removed */
	p += d;

	/* Copy remaining features */
	if (strlen(p)) {
		while (*p == ' ')
			p++;
		if (strlen(p)) {
			p--;
			strcat(n, p);
		}
	}

out:
	FREE(*f);
	*f = n;

	return 0;
}
