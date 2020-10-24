static char *seek_delim(const char *s)
{
	const char *p = s;
	char *delim;

	delim = strpbrk(p, "(, \t:");
	if (delim && *delim != ':' && (delim == s || *(delim - 1) != '\\'))
		return delim;

	while (*p) {
		if (*p != ':') {
			p++;
			continue;
		}
		if (!strncmp(p, ":/", 2))
			return (char *) p;
		p++;
	}

	return NULL;
}