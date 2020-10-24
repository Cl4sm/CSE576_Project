static int parse_nfs(struct autofs_point *ap,
		     const char *filsysline, const char *name,
		     int name_len, char *source, int source_len,
		     char *options, int options_len)
{
	const char *p;
	char mount[HESIOD_LEN + 1];
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

	/* Isolate the remote mountpoint for this NFS fs. */
	for (i = 0; (!isspace(p[i]) && i < ((int) sizeof(mount) - 1)); i++) {
		if (!p[i]) {
			error(ap->logopt, MODPREFIX
			      "unexpeced end of input looking for NFS "
			      "mountpoint: %s", p);
			return 1;
		}
		mount[i] = p[i];
	}

	mount[i] = 0;
	p += i;

	/* Skip whitespace. */
	while ((*p) && (isspace(*p)))
		p++;

	/* Isolate the remote host. */
	for (i = 0; (!isspace(p[i]) && i < source_len); i++) {
		if (!p[i]) {
			error(ap->logopt, MODPREFIX
			      "unexpeced end of input looking for NFS "
			      "host: %s", p);
			return 1;
		}
		source[i] = p[i];
	}

	source[i] = 0;
	p += i;

	if (strlen(source) + strlen(mount) + 2 > source_len) {
		error(ap->logopt, MODPREFIX "entry too log for mount source");
		return 1;
	}

	/* Append ":mountpoint" to the source to get "host:mountpoint". */
	strcat(source, ":");
	strcat(source, mount);

	/* Skip whitespace. */
	while ((*p) && (isspace(*p)))
		p++;

	/* Isolate the mount options. */
	for (i = 0; (!isspace(p[i]) && i < options_len); i++) {
		if (!p[i]) {
			error(ap->logopt, MODPREFIX
			      "unexpeced end of input looking for NFS "
			      "mount options: %s", p);
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
	      "parsing NFS record gives '%s'->'%s' with options" "'%s'",
	      name, source, options);

	return 0;
}