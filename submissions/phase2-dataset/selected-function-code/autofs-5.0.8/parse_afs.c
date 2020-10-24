static int parse_afs(struct autofs_point *ap,
		     const char *filsysline, const char *name, int name_len,
		     char *source, int source_len, char *options, int options_len)
{
	const char *p;
	int i;

	p = filsysline;

	/* Skip whitespace. */
	while (isspace(*p))
		p++;

	/* Skip the filesystem type. */
	while (!isspace(*p))
		p++;

	/* Skip whitespace. */
	while (isspace(*p))
		p++;

	/* Isolate the source for this AFS fs. */
	for (i = 0; (!isspace(p[i]) && i < source_len); i++) {
		if (!p[i]) {
			error(ap->logopt, MODPREFIX
			      "unexpeced end of input looking for AFS "
			      "source: %s", p);
			return 1;
		}
		source[i] = p[i];
	}

	source[i] = 0;
	p += i;

	/* Skip whitespace. */
	while ((*p) && (isspace(*p)))
		p++;

	/* Isolate the options for this AFS fs. */
	for (i = 0; (!isspace(p[i]) && i < options_len); i++) {
		if (!p[i]) {
			error(ap->logopt, MODPREFIX
			      "unexpeced end of input looking for AFS "
			      "options: %s", p);
			return 1;
		}
		options[i] = p[i];
	}
	options[i] = 0;

	/* Hack for "r" or "w" options. */
	if (!strcmp(options, "r"))
		strcpy(options, "ro");

	if (!strcmp(options, "w"))
		strcpy(options, "rw");

	debug(ap->logopt,
	      MODPREFIX
	      "parsing AFS record gives '%s'->'%s' with options" " '%s'",
	      name, source, options);

	return 0;
}