renv(const char *host, char **aname, char **apass)
{
	register char *cp;
	char *comma;

	cp = renvlook(host);
	if (cp == NULL)
		return;
	if (!isalpha(cp[0]))
		return;
	comma = index(cp, ',');
	if (comma == 0)
		return;
	if (*aname == 0) {
		*aname = (char*)malloc(comma - cp + 1);
		strncpy(*aname, cp, comma - cp);
	} else
		if (strncmp(*aname, cp, comma - cp))
			return;
	comma++;
	cp = (char *)malloc(strlen(comma)+1);
	strcpy(cp, comma);
	*apass = (char*)malloc(16);
	mkpwclear(cp, host[0], *apass);
}
