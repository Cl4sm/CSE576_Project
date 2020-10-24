extern int
snprint_defaults (char * buff, int len)
{
	int fwd = 0;
	int i;
	struct keyword *rootkw;
	struct keyword *kw;

	rootkw = find_keyword(NULL, "defaults");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "defaults {\n");
	if (fwd > len)
		return len;

	iterate_sub_keywords(rootkw, kw, i) {
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t%k %v\n",
				kw, NULL);
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "}\n");
	if (fwd > len)
		return len;
	return fwd;
}
