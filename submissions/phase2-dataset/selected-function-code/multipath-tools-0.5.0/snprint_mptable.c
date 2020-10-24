snprint_mptable (char * buff, int len, vector mptable)
{
	int fwd = 0;
	int i;
	struct mpentry * mpe;
	struct keyword * rootkw;

	rootkw = find_keyword(NULL, "multipaths");
	if (!rootkw)
		return 0;

	fwd += snprintf(buff + fwd, len - fwd, "multipaths {\n");
	if (fwd > len)
		return len;
	vector_foreach_slot (mptable, mpe, i) {
		fwd += snprint_mpentry(buff + fwd, len - fwd, mpe);
		if (fwd > len)
			return len;
	}
	fwd += snprintf(buff + fwd, len - fwd, "}\n");
	if (fwd > len)
		return len;
	return fwd;
}
