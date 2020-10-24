static int do_mkdir(const char *parent, const char *path, mode_t mode)
{
	int status;
	struct stat st;
	struct statfs fs;

	/* If path exists we're done */
	status = stat(path, &st);
	if (status == 0) {
		if (!S_ISDIR(st.st_mode))
			errno = ENOTDIR;
		errno = EEXIST;
		return 0;
	}

	/*
	 * If we're trying to create a directory within an autofs fs
	 * or the path is contained in a localy mounted fs go ahead.
	 */
	status = -1;
	if (*parent)
		status = statfs(parent, &fs);
	if ((status != -1 && fs.f_type == (__SWORD_TYPE) AUTOFS_SUPER_MAGIC) ||
	    contained_in_local_fs(path)) {
		mode_t mask = umask(0022);
		int ret = mkdir(path, mode);
		(void) umask(mask);
		if (ret == -1) {
			errno = EACCES;
			return 0;
		}
		return 1;
	}

	errno = EACCES;
	return 0;
}