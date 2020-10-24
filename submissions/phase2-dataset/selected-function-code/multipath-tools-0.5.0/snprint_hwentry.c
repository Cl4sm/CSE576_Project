static int
snprint_hwentry (char * buff, int len, struct hwentry * hwe)
{
	int i;
	int fwd = 0;
	struct keyword * kw;
	struct keyword * rootkw;

	rootkw = find_keyword(NULL, "devices");

	if (!rootkw || !rootkw->sub)
		return 0;

	rootkw = find_keyword(rootkw->sub, "device");

	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "\tdevice {\n");
	if (fwd > len)
		return len;
	iterate_sub_keywords(rootkw, kw, i) {
		fwd += snprint_keyword(buff + fwd, len - fwd, "\t\t%k %v\n",
				kw, hwe);
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "\t}\n");
	if (fwd > len)
		return len;
	return fwd;
}
