int mkdir_path(const char *path, mode_t mode)
{
	char buf[PATH_MAX];
	char parent[PATH_MAX];
	const char *cp = path, *lcp = path;
	char *bp = buf, *pp = parent;

	*parent = '\0';

	do {
		if (cp != path && (*cp == '/' || *cp == '\0')) {
			memcpy(bp, lcp, cp - lcp);
			bp += cp - lcp;
			*bp = '\0';
			if (!do_mkdir(parent, buf, mode)) {
				if (*cp != '\0') {
					memcpy(pp, lcp, cp - lcp);
					pp += cp - lcp;
					*pp = '\0';
					lcp = cp;
					continue;
				}
				return -1;
			}
			memcpy(pp, lcp, cp - lcp);
			pp += cp - lcp;
			*pp = '\0';
			lcp = cp;
		}
	} while (*cp++ != '\0');

	return 0;
}