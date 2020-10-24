char *
varsub(s)
	char *s;
{
	int rm = 8;		/* length of result string space */
	char *r;		/* result string */
	int rl = 0;
	char *p;
	char *vn, *vv;
	char c;
	int l;

	r = TALLOC(rm, char, "var");
	while (*s) {
		for (p = s; *p && *p != '$'; p++) ;
		if (l = p - s) {
			if (rl + l >= rm) {
				rm = rl + l + 1;
				r = TREALLOC(r, rm, char);
			}
			strncpy(r + rl, s, l);
			rl += l;
		}
		s = p++;
		if (*s == '$') {
			if (*p == '{')
				p++;
			vn = p;
			while (isalnum(*p) || *p == '_')
				p++;
			c = *p;
			*p = 0;

			vv = getenv(vn);

#ifdef WIN32
			if (!vv)
				vv = (char *) read_pvmregistry(vn);
#endif

			*p = c;
			if (*p == '}')
				p++;
			if (vv)
				l = strlen(vv);
			else {
				vv = s;
				l = p - s;
			}
			if (l) {
				if (rl + l >= rm) {
					rm = rl + l + 1;
					r = TREALLOC(r, rm, char);
				}
				strncpy(r + rl, vv, l);
				rl += l;
			}
			s = p;
		}
	}
	r[rl] = 0;
	return r;
}
