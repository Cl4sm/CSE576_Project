int
file_loop(register FILE *f, file_process fn, register const char *name)
{
	register int n;
	register char *cp, *cp2, *h;
	u_int32_t a;
	register time_t t;
	register struct hostent *hp;
	char *interface = NULL;
	char line[1024];
	u_char e[6];

	n = 0;
	while (fgets(line, sizeof(line), f)) {
		++n;
		cp = line;
		cp2 = cp + strlen(cp) - 1;
		if (cp2 >= cp && *cp2 == '\n')
			*cp2++ = '\0';
		if (*cp == '#')
			continue;
		if ((cp2 = strchr(cp, '\t')) == NULL) {
			syslog(LOG_ERR, "file_loop: %s:%d syntax error #1",
			    name, n);
fprintf(stderr, "file_loop: %s:%d syntax error #1\n", name, n);
			continue;
		}

		/* Ethernet address comes first */
		*cp2++ = '\0';
		if (!str2e(cp, e)) {
			syslog(LOG_ERR,
			    "file_loop: %s:%d bad ether addr \"%s\"",
			    name, n, cp);
fprintf(stderr, "file_loop: %s:%d bad ether addr \"%s\"\n", name, n, cp);
			continue;
		}

		/* ip address is next */
		cp = cp2;
		if ((cp2 = strchr(cp, '\t')) != NULL)
			*cp2++ = '\0';
		if (!isdigit((int)*cp) || (int32_t)(a = inet_addr(cp)) == -1) {
			if ((hp = gethostbyname(cp)) == NULL) {
				syslog(LOG_ERR,
				    "file_loop: %s:%d bad hostname \"%s\"",
				    name, n, cp);
fprintf(stderr, "file_loop: %s:%d bad hostname \"%s\"\n", name, n, cp);
				continue;
			}
			BCOPY(hp->h_addr, &a, 4);
		}

		/* timestamp and hostname are optional */
		if (cp2 == NULL) {
			t = 0;
			h = NULL;
		} else {
			t = atoi(cp2);
			h = strchr(cp2, '\t');
			if (h != NULL) {
				++h;
				++cp2;
				while (*cp2 != '\n' && *cp2 != '\t' &&
				       *cp2 != '\0')
					++cp2;
				if (*cp2 == '\t') {
					*cp2++ = '\0';
					while (*cp2 != '\n' && *cp2 != '\t' &&
					       *cp2 != '\0') ++cp2;
					if (*cp2 == '\t') {
						*cp2++ = '\0';
						interface = cp2;
						while (*cp2 != '\n' && *cp2 != '\t' &&
						       *cp2 != '\0') ++cp2;
					}
				}
				*cp2 = '\0';
			}
		}

		if (!(*fn)(a, e, t, h, interface))
			return(0);
	}

	return(1);
}