ensure_directories_exist(char *str, mode_t dir_mode)
{
	char *pathname;
	char *end;
	int err;

	pathname = strdup(str);
	if (!pathname){
		condlog(0, "Cannot copy file pathname %s : %s",
			str, strerror(errno));
		return -1;
	}
	end = pathname;
	/* skip leading slashes */
	while (end && *end && (*end == '/'))
		end++;

	while ((end = strchr(end, '/'))) {
		/* if there is another slash, make the dir. */
		*end = '\0';
		err = mkdir(pathname, dir_mode);
		if (err && errno != EEXIST) {
			condlog(0, "Cannot make directory [%s] : %s",
				pathname, strerror(errno));
			free(pathname);
			return -1;
		}
		if (!err)
			condlog(3, "Created dir [%s]", pathname);
		*end = '/';
		end++;
	}
	free(pathname);
	return 0;
}
