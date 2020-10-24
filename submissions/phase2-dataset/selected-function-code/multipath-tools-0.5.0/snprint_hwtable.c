snprint_hwtable (char * buff, int len, vector hwtable)
{
	int fwd = 0;
	int i;
	struct hwentry * hwe;
	struct keyword * rootkw;

	rootkw = find_keyword(NULL, "devices");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "devices {\n");
	if (fwd > len)
		return len;
	vector_foreach_slot (hwtable, hwe, i) {
		fwd += snprint_hwentry(buff + fwd, len - fwd, hwe);
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "}\n");
	if (fwd > len)
		return len;
	return fwd;
}
