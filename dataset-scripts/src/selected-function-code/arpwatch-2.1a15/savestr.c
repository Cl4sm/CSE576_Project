char *
savestr(register const char *str)
{
	register int i;
	register char *cp;
	static char *strptr = NULL;
	static int strsize = 0;

	i = strlen(str) + 1;
	if (i > strsize) {
		strsize = 512;
		strptr = malloc(strsize);
		if (strptr == NULL) {
			syslog(LOG_ERR, "savestr(): malloc: %m");
			exit(1);
		}
		memset(strptr, 0, strsize);
	}
	(void)strcpy(strptr, str);
	cp = strptr;
	strptr += i;
	strsize -= i;
	return (cp);
}