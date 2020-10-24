static int walk_tree(const char *base, int (*fn) (unsigned logopt,
						  const char *file,
						  const struct stat * st,
						  int, void *), int incl, unsigned logopt, void *arg)
{
	char buf[PATH_MAX + 1];
	struct stat st, *pst = &st;
	int ret;

	if (!is_mounted(_PATH_MOUNTED, base, MNTS_REAL))
		ret = lstat(base, pst);
	else {
		pst = NULL;
		ret = 0;
	}

	if (ret != -1 && (fn) (logopt, base, pst, 0, arg)) {
		if (S_ISDIR(st.st_mode)) {
			struct dirent **de;
			int n;

			n = scandir(base, &de, 0, alphasort);
			if (n < 0)
				return -1;

			while (n--) {
				int ret, size;

				if (strcmp(de[n]->d_name, ".") == 0 ||
				    strcmp(de[n]->d_name, "..") == 0) {
					free(de[n]);
					continue;
				}

				size = sizeof(buf);
				ret = cat_path(buf, size, base, de[n]->d_name);
				if (!ret) {
					do {
						free(de[n]);
					} while (n--);
					free(de);
					return -1;
				}

				walk_tree(buf, fn, 1, logopt, arg);
				free(de[n]);
			}
			free(de);
		}
		if (incl)
			(fn) (logopt, base, pst, 1, arg);
	}
	return 0;
}