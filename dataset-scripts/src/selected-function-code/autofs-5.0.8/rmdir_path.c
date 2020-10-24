int rmdir_path(struct autofs_point *ap, const char *path, dev_t dev)
{
	int len = strlen(path);
	char buf[PATH_MAX];
	char *cp;
	int first = 1;
	struct stat st;
	struct statfs fs;

	strcpy(buf, path);
	cp = buf + len;

	do {
		*cp = '\0';

		/*
		 *  Before removing anything, perform some sanity checks to
		 *  ensure that we are looking at files in the automount
		 *  file system.
		 */
		memset(&st, 0, sizeof(st));
		if (lstat(buf, &st) != 0) {
			crit(ap->logopt, "lstat of %s failed", buf);
			return -1;
		}

		/* Termination condition removing full path within autofs fs */
		if (st.st_dev != dev)
			return 0;

		if (statfs(buf, &fs) != 0) {
			error(ap->logopt, "could not stat fs of %s", buf);
			return -1;
		}

		if (fs.f_type != (__SWORD_TYPE) AUTOFS_SUPER_MAGIC) {
			crit(ap->logopt, "attempt to remove directory from a "
			     "non-autofs filesystem!");
			crit(ap->logopt,
			     "requestor dev == %llu, \"%s\" owner dev == %llu",
			     dev, buf, st.st_dev);
			return -1;
		}
			     
		/*
		 * Last element of path may be a symbolic link; all others
		 * are directories (and the last directory element is
		 * processed first, hence the variable name)
		 */
		if (rmdir(buf) == -1) {
			if (first && errno == ENOTDIR) {
				/*
				 *  Ensure that we will only remove
				 *  symbolic links.
				 */
				if (S_ISLNK(st.st_mode)) {
					if (unlink(buf) == -1)
						return -1;
				} else {
					crit(ap->logopt,
					   "file \"%s\" is neither a directory"
					   " nor a symbolic link. mode %d",
					   buf, st.st_mode);
					return -1;
				}
			}

			/*
			 *  If we fail to remove a directory for any reason,
			 *  we need to return an error.
			 */
			return -1;
		}

		first = 0;
	} while ((cp = strrchr(buf, '/')) != NULL && cp != buf);

	return 0;
}