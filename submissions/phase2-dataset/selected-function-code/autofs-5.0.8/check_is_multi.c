static int check_is_multi(const char *mapent)
{
	const char *p = mapent;
	int multi = 0;
	int not_first_chunk = 0;

	if (!p) {
		logerr(MODPREFIX "unexpected NULL map entry pointer");
		return 0;
	}

	if (*p == '"')
		p++;

	/* If first character is "/" it's a multi-mount */
	if (*p == '/')
		return 1;

	while (*p) {
		p = skipspace(p);

		/*
		 * After the first chunk there can be additional
		 * locations (possibly not multi) or possibly an
		 * options string if the first entry includes the
		 * optional '/' (is multi). Following this any
		 * path that begins with '/' indicates a mutil-mount
		 * entry.
		 */
		if (not_first_chunk) {
			if (*p == '"')
				p++;
			if (*p == '/' || *p == '-') {
				multi = 1;
				break;
			}
		}

		while (*p == '-') {
			p += chunklen(p, 0);
			p = skipspace(p);
		}

		/*
		 * Expect either a path or location
		 * after which it's a multi mount.
		 */
		p += chunklen(p, check_colon(p));
		not_first_chunk++;
	}

	return multi;
}