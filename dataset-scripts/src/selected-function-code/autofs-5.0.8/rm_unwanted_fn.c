static int rm_unwanted_fn(unsigned logopt, const char *file, const struct stat *st, int when, void *arg)
{
	dev_t dev = *(dev_t *) arg;
	char buf[MAX_ERR_BUF];
	struct stat newst;

	if (!st)
		return 0;

	if (when == 0) {
		if (st->st_dev != dev)
			return 0;
		return 1;
	}

	if (lstat(file, &newst)) {
		crit(logopt, "unable to stat file, possible race condition");
		return 0;
	}

	if (newst.st_dev != dev) {
		crit(logopt, "file %s has the wrong device, possible race condition",
		     file);
		return 0;
	}

	if (S_ISDIR(newst.st_mode)) {
		debug(logopt, "removing directory %s", file);
		if (rmdir(file)) {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			warn(logopt,
			      "unable to remove directory %s: %s", file, estr);
			return 0;
		}
	} else if (S_ISREG(newst.st_mode)) {
		crit(logopt, "attempting to remove files from a mounted "
		     "directory. file %s", file);
		return 0;
	} else if (S_ISLNK(newst.st_mode)) {
		debug(logopt, "removing symlink %s", file);
		unlink(file);
	}
	return 1;
}