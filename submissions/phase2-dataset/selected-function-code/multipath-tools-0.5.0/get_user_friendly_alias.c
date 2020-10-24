char *
get_user_friendly_alias(char *wwid, char *file, char *prefix,
			int bindings_read_only)
{
	char *alias;
	int fd, id;
	FILE *f;
	int can_write;

	if (!wwid || *wwid == '\0') {
		condlog(3, "Cannot find binding for empty WWID");
		return NULL;
	}

	fd = open_file(file, &can_write, BINDINGS_FILE_HEADER);
	if (fd < 0)
		return NULL;

	f = fdopen(fd, "r");
	if (!f) {
		condlog(0, "cannot fdopen on bindings file descriptor : %s",
			strerror(errno));
		close(fd);
		return NULL;
	}

	id = lookup_binding(f, wwid, &alias, prefix);
	if (id < 0) {
		fclose(f);
		return NULL;
	}

	if (fflush(f) != 0) {
		condlog(0, "cannot fflush bindings file stream : %s",
			strerror(errno));
		fclose(f);
		return NULL;
	}

	if (!alias && can_write && !bindings_read_only && id)
		alias = allocate_binding(fd, wwid, id, prefix);

	fclose(f);
	return alias;
}
