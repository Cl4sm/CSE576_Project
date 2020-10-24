static int parse_node_map_wrap(const char *file_name,
			       int (*create) (void *, uint64_t, char *),
			       void *cxt,
			       char *linebuf,
			       unsigned int linebuflen)
{
	char line[PARSE_NODE_MAP_BUFLEN];
	FILE *f;

	if (!(f = fopen(file_name, "r")))
		return -1;

	while (fgets(line, sizeof(line), f)) {
		uint64_t guid;
		char *p, *e;

		p = line;
		while (isspace(*p))
			p++;
		if (*p == '\0' || *p == '\n' || *p == '#')
			continue;

		guid = strtoull(p, &e, 0);
		if (e == p || (!isspace(*e) && *e != '#' && *e != '\0')) {
			fclose(f);
			errno = EIO;
			if (linebuf) {
				memcpy(linebuf, line,
				       MIN(PARSE_NODE_MAP_BUFLEN, linebuflen));
				e = strpbrk(linebuf, "\n");
				if (e)
					*e = '\0';
			}
			return -1;
		}

		p = e;
		while (isspace(*p))
			p++;

		e = strpbrk(p, "\n");
		if (e)
			*e = '\0';

		if (create(cxt, guid, p)) {
			fclose(f);
			return -1;
		}
	}

	fclose(f);
	return 0;
}