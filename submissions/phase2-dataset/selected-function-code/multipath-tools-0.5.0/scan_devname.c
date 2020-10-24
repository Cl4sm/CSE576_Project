scan_devname(char *alias, char *prefix)
{
	char *c;
	int i, n = 0;

	if (!prefix || strncmp(alias, prefix, strlen(prefix)))
		return -1;

	if (strlen(alias) == strlen(prefix))
		return -1;	

	if (strlen(alias) > strlen(prefix) + 7)
		/* id of 'aaaaaaaa' overflows int */
		return -1;

	c = alias + strlen(prefix);
	while (*c != '\0' && *c != ' ' && *c != '\t') {
		if (*c < 'a' || *c > 'z')
			return -1;
		i = *c - 'a';
		n = ( n * 26 ) + i;
		if (n < 0)
			return -1;
		c++;
		n++;
	}

	return n;
}
