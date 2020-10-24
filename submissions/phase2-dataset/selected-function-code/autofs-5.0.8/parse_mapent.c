static int parse_mapent(const char *ent, char *g_options, char **options, char **location, int logopt)
{
	char buf[MAX_ERR_BUF];
	const char *p;
	char *myoptions, *loc;
	int l;

	p = ent;

	myoptions = strdup(g_options);
	if (!myoptions) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		error(logopt, MODPREFIX "strdup: %s", estr);
		return 0;
	}

	/* Local options are appended to per-map options */
	if (*p == '-') {
		do {
			char *tmp, *newopt = NULL;

			p = parse_options(p, &newopt, logopt);
			if (newopt && strstr(newopt, myoptions)) {
				free(myoptions);
				myoptions = newopt;
			} else {
				tmp = concat_options(myoptions, newopt);
				if (!tmp) {
					char *estr;
					estr = strerror_r(errno, buf, MAX_ERR_BUF);
					error(logopt, MODPREFIX
					      "concat_options: %s", estr);
					if (newopt)
						free(newopt);
					free(myoptions);
					return 0;
				}
				myoptions = tmp;
			}

			p = skipspace(p);
		} while (*p == '-');
	}

	debug(logopt, MODPREFIX "gathered options: %s", myoptions);

	l = chunklen(p, check_colon(p));
	loc = dequote(p, l, logopt);
	if (!loc) {
		warn(logopt, MODPREFIX "possible missing location");
		free(myoptions);
		return 0;
	}

	/* Location can't begin with a '/' */
	if (*p == '/') {
		warn(logopt, MODPREFIX "error location begins with \"/\"");
		free(myoptions);
		free(loc);
		return 0;
	}

	if (!validate_location(logopt, loc)) {
		free(myoptions);
		free(loc);
		return 0;
	}

	debug(logopt, MODPREFIX "dequote(\"%.*s\") -> %s", l, p, loc);

	p += l;
	p = skipspace(p);

	while (*p && ((*p == '"' && *(p + 1) != '/') || (*p != '"' && *p != '/'))) {
		char *tmp, *ent_chunk;

		l = chunklen(p, check_colon(p));
		ent_chunk = dequote(p, l, logopt);
		if (!ent_chunk) {
			warn(logopt, MODPREFIX "null location or out of memory");
			free(myoptions);
			free(loc);
			return 0;
		}

		/* Location can't begin with a '/' */
		if (*p == '/') {
			warn(logopt,
			      MODPREFIX "error location begins with \"/\"");
			free(ent_chunk);
			free(myoptions);
			free(loc);
			return 0;
		}

		if (!validate_location(logopt, ent_chunk)) {
			free(ent_chunk);
			free(myoptions);
			free(loc);
			return 0;
		}

		debug(logopt, MODPREFIX "dequote(\"%.*s\") -> %s", l, p, ent_chunk);

		tmp = realloc(loc, strlen(loc) + l + 2);
		if (!tmp) {
			error(logopt, MODPREFIX "out of memory");
			free(ent_chunk);
			free(myoptions);
			free(loc);
			return 0;
		}
		loc = tmp;

		strcat(loc, " ");
		strcat(loc, ent_chunk);

		free(ent_chunk);

		p += l;
		p = skipspace(p);
	}

	*options = myoptions;
	*location = loc;

	return (p - ent);
}