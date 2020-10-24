int
get_user_friendly_wwid(char *alias, char *buff, char *file)
{
	int fd, unused;
	FILE *f;

	if (!alias || *alias == '\0') {
		condlog(3, "Cannot find binding for empty alias");
		return -1;
	}

	fd = open_file(file, &unused, BINDINGS_FILE_HEADER);
	if (fd < 0)
		return -1;

	f = fdopen(fd, "r");
	if (!f) {
		condlog(0, "cannot fdopen on bindings file descriptor : %s",
			strerror(errno));
		close(fd);
		return -1;
	}

	rlookup_binding(f, buff, alias);
	if (!strlen(buff)) {
		fclose(f);
		return -1;
	}

	fclose(f);
	return 0;
}
