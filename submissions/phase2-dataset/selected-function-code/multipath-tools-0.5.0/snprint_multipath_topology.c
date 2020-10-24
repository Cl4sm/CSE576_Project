extern int
snprint_multipath_topology (char * buff, int len, struct multipath * mpp,
			    int verbosity)
{
	int j, i, fwd = 0;
	struct path * pp = NULL;
	struct pathgroup * pgp = NULL;
	char style[64];
	char * c = style;
	char fmt[64];
	char * f;

	if (verbosity <= 0)
		return fwd;

	reset_multipath_layout();

	if (verbosity == 1)
		return snprint_multipath(buff, len, "%n", mpp);

	if(isatty(1))
		c += sprintf(c, "%c[%dm", 0x1B, 1); /* bold on */

	if (verbosity > 1 &&
	    mpp->action != ACT_NOTHING &&
	    mpp->action != ACT_UNDEF)
			c += sprintf(c, "%%A: ");

	c += sprintf(c, "%%n");

	if (strncmp(mpp->alias, mpp->wwid, WWID_SIZE))
		c += sprintf(c, " (%%w)");

	c += sprintf(c, " %%d %%s");
	if(isatty(1))
		c += sprintf(c, "%c[%dm", 0x1B, 0); /* bold off */

	fwd += snprint_multipath(buff + fwd, len - fwd, style, mpp);
	if (fwd > len)
		return len;
	fwd += snprint_multipath(buff + fwd, len - fwd, PRINT_MAP_PROPS, mpp);
	if (fwd > len)
		return len;

	if (!mpp->pg)
		return fwd;

	vector_foreach_slot (mpp->pg, pgp, j) {
		f=fmt;
		pgp->selector = mpp->selector; /* hack */
		if (j + 1 < VECTOR_SIZE(mpp->pg)) {
			strcpy(f, "|-+- " PRINT_PG_INDENT);
		} else
			strcpy(f, "`-+- " PRINT_PG_INDENT);
		fwd += snprint_pathgroup(buff + fwd, len - fwd, fmt, pgp);
		if (fwd > len)
			return len;

		vector_foreach_slot (pgp->paths, pp, i) {
			f=fmt;
			if (*f != '|')
				*f=' ';
			f++;
			if (i + 1 < VECTOR_SIZE(pgp->paths))
				strcpy(f, " |- " PRINT_PATH_INDENT);
			else
				strcpy(f, " `- " PRINT_PATH_INDENT);
			fwd += snprint_path(buff + fwd, len - fwd, fmt, pp);
			if (fwd > len)
				return len;
		}
	}
	return fwd;
}
