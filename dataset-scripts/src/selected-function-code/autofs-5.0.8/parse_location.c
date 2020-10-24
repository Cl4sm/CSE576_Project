int parse_location(unsigned logopt, struct host **hosts,
		   const char *list, unsigned int options)
{
	char *str, *p, *delim;
	unsigned int empty = 1;

	if (!list)
		return 0;

	str = strdup(list);
	if (!str)
		return 0;

	p = str;

	while (p && *p) {
		char *next = NULL;
		int weight = 0;

		p += strspn(p, " \t,");
		delim = seek_delim(p);

		if (delim) {
			if (*delim == '(') {
				char *w = delim + 1;

				*delim = '\0';

				delim = strchr(w, ')');
				if (delim) {
					*delim = '\0';
					weight = atoi(w);
				}
				else {
					/* syntax error - Mismatched brackets */
					free_host_list(hosts);
					free(str);
					return 0;
				}
				delim++;
			}

			if (*delim == ':') {
				char *path;

				*delim = '\0';
				path = delim + 1;

				/* Oh boy - might have spaces in the path */
				next = path;
				while (*next && strncmp(next, ":/", 2))
					next++;

				/* No spaces in host names at least */
				if (*next == ':') {
					while (*next &&
					      (*next != ' ' && *next != '\t'))
						next--;
					*next++ = '\0';
				}

				if (p != delim) {
					if (!add_host_addrs(hosts, p, weight, options)) {
						if (empty) {
							p = next;
							continue;
						}
					}

					if (!add_path(*hosts, path, strlen(path))) {
						free_host_list(hosts);
						free(str);
						return 0;
					}
				} else {
					if (!add_local_path(hosts, path)) {
						p = next;
						continue;
					}
				}
			} else if (*delim != '\0') {
				*delim = '\0';
				next = delim + 1;

				if (!add_host_addrs(hosts, p, weight, options)) {
					p = next;
					continue;
				}

				empty = 0;
			}
		} else {
			/* syntax error - no mount path */
			free_host_list(hosts);
			free(str);
			return 0;
		}

		p = next;
	}

	free(str);
	return 1;
}