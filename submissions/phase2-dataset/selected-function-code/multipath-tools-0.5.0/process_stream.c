int
process_stream(vector keywords)
{
	int i;
	int r = 0;
	struct keyword *keyword;
	char *str;
	char *buf;
	vector strvec;
	vector uniques;

	uniques = vector_alloc();
	if (!uniques)
		return 1;

	buf = MALLOC(MAXBUF);

	if (!buf) {
		vector_free(uniques);
		return 1;
	}

	while (read_line(buf, MAXBUF)) {
		line_nr++;
		strvec = alloc_strvec(buf);
		memset(buf,0, MAXBUF);

		if (!strvec)
			continue;

		str = VECTOR_SLOT(strvec, 0);

		if (!strcmp(str, EOB) && kw_level > 0) {
			free_strvec(strvec);
			break;
		}

		for (i = 0; i < VECTOR_SIZE(keywords); i++) {
			keyword = VECTOR_SLOT(keywords, i);

			if (!strcmp(keyword->string, str)) {
				if (keyword->unique &&
				    warn_on_duplicates(uniques, str)) {
						r = 1;
						free_strvec(strvec);
						goto out;
				}
				if (keyword->handler)
					r += (*keyword->handler) (strvec);

				if (keyword->sub) {
					kw_level++;
					r += process_stream(keyword->sub);
					kw_level--;
				}
				break;
			}
		}
		if (i >= VECTOR_SIZE(keywords))
			condlog(1, "multipath.conf +%d, invalid keyword: %s",
				line_nr, str);

		free_strvec(strvec);
	}

out:
	FREE(buf);
	free_uniques(uniques);
	return r;
}
