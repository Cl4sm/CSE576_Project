void
ruserpass(const char *host, char **aname, char **apass)
{
	static char namebuf [256];
	struct hostent *hp;
	char    name[256]; /* a temp name buffer to avoid overlayyed */

	strncpy(name, host, sizeof(name) - 1);

	if (hp = gethostbyname (name))
		strncpy(name,hp->h_name, sizeof(name) - 1);
	renv(name, aname, apass);
	if (*aname == 0 || *apass == 0)
		rnetrc(name,aname,apass); /*rnetrc would call gethostbyname */
	if (*aname == 0) {
		*aname = getlogin();
		if (*aname == NULL) {
			struct passwd *pp = getpwuid(getuid());
			if (pp != NULL)
				*aname = pp->pw_name;
		}
		printf("Name (%s:%s): ", host, *aname);
		fflush(stdout);
		if (read(2, namebuf, sizeof (namebuf)) <= 0) {
			perror ("read");
			exit(1);
		}
		if (namebuf [0] != '\n') {
			char *ptr;
			*aname = namebuf;
			namebuf [sizeof (namebuf) - 1] = '0';
			if (ptr = index(namebuf, '\n'))
				*ptr = 0;
		}
	}
	if (*aname && *apass == 0) {
		printf("Password (%s:%s): ", host, *aname);
		fflush(stdout);
		*apass = getpass("");
	}
}
