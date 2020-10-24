int
ec_loop(register FILE *f, ec_process fn, register const char *nm)
{
	register int n;
	register char *cp, *cp2, *text;
	register int sawblank;
	u_int32_t o;
	char line[1024];

	n = 0;
	while (fgets(line, sizeof(line), f)) {
		++n;
		cp = line;
		cp2 = cp + strlen(cp) - 1;
		if (cp2 >= cp && *cp2 == '\n')
			*cp2++ = '\0';
		if (*cp == '#')
			continue;
		if ((cp2 = strchr(cp, '\t')) == 0) {
			syslog(LOG_ERR, "ec_loop(): %s:%d missing tab", nm, n);
			continue;
		}

		/* 3 octets come first */
		*cp2++ = '\0';
		text = cp2;
		if (!ec_a2o(cp, &o)) {
			syslog(LOG_ERR, "ec_loop(): %s:%d bad octets", nm, n);
			continue;
		}

		/* Compress blanks */
		cp = cp2 = text;
		sawblank = 0;
		while (*cp != '\0') {
			if (sawblank) {
				*cp2++ = ' ';
				sawblank = 0;
			}
			*cp2++ = *cp++;
			while (isspace((int)*cp)) {
				++cp;
				sawblank = 1;
			}
		}
		*cp2 = '\0';

		if (!(*fn)(o, text))
			return (0);
	}

	return (1);
}