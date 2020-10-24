snprint_mpentry (char * buff, int len, struct mpentry * mpe)
{
	int i;
	int fwd = 0;
	struct keyword * kw;
	struct keyword * rootkw;

	rootkw = find_keyword(NULL, "multipath");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "\tmultipath {\n");
	if (fwd > len)
		return len;
	iterate_sub_keywords(rootkw, kw, i) {
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				kw, mpe);
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "\t}\n");
	if (fwd > len)
		return len;
	return fwd;
}
