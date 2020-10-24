crunchzap(s, acp, av)
	char *s;		/* the string to parse */
	int *acp;		/* max words in, ac out */
	char **av;		/* pointers to args */
{
	register int ac;
	register char *p = s;
	register int n = *acp;

	/* separate out words of command */

	ac = 0;
	while (*p) {
		while (*p && *p++ != '<');
		if (*p) {
			if (ac >= n) {
	/* command too long */
				*acp = ac;
				return 1;
			}
			av[ac++] = p;
			while (*p && *p != '>') p++;
			if (*p) *p++ = 0;
		}
	}
	*acp = ac;
	return 0;
}
