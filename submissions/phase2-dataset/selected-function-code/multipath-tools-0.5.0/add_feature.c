extern int
add_feature (char **f, char *n)
{
	int c = 0, d, l;
	char *e, *p, *t;

	if (!f)
		return 1;

	/* Nothing to do */
	if (!n || *n == '0')
		return 0;

	/* Check if feature is already present */
	if (strstr(*f, n))
		return 0;

	/* Get feature count */
	c = strtoul(*f, &e, 10);
	if (*f == e)
		/* parse error */
		return 1;

	/* Check if we need to increase feature count space */
	l = strlen(*f) + strlen(n) + 1;

	/* Count new features */
	if ((c % 10) == 9)
		l++;
	c++;
	p = n;
	while (*p != '\0') {
		if (*p == ' ' && p[1] != '\0' && p[1] != ' ') {
			if ((c % 10) == 9)
				l++;
			c++;
		}
		p++;
	}

	t = MALLOC(l + 1);
	if (!t)
		return 1;

	memset(t, 0, l + 1);

	/* Update feature count */
	d = c;
	l = 1;
	while (d > 9) {
		d /= 10;
		l++;
	}
	p = t;
	snprintf(p, l + 2, "%0d ", c);

	/* Copy the feature string */
	p = strchr(*f, ' ');
	if (p) {
		while (*p == ' ')
			p++;
		strcat(t, p);
		strcat(t, " ");
	} else {
		p = t + strlen(t);
	}
	strcat(t, n);

	FREE(*f);
	*f = t;

	return 0;
}
