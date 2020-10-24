static char *automount_path_to_fifo(unsigned logopt, const char *path)
{
	char *fifo_name, *p;
	int  name_len = strlen(path) + strlen(fifodir) + 1;
	int ret;

	fifo_name = malloc(name_len);
	if (!fifo_name)
		return NULL;
	ret = snprintf(fifo_name, name_len, "%s%s", fifodir, path);
	if (ret >= name_len) {
		info(logopt,
		     "fifo path for \"%s\" truncated to \"%s\".  This may "
		     "lead to --set-log-priority commands being sent to the "
		     "wrong automount daemon.", path, fifo_name);
	}

	/*
	 *  An automount path can be made up of subdirectories.  So, to
	 *  create the fifo name, we will just replace instances of '/' with
	 *  '-'. 
	 */
	p = fifo_name + strlen(fifodir);
	while (*p != '\0') {
		if (*p == '/')
			*p = '-';
		p++;
	}

	debug(logopt, "fifo name %s",fifo_name);

	return fifo_name;
}