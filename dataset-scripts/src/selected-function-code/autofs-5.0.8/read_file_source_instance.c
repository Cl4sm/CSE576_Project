static int read_file_source_instance(struct autofs_point *ap, struct map_source *map, time_t age)
{
	struct map_source *instance;
	char src_file[] = "file";
	char src_prog[] = "program";
	struct stat st;
	char *type, *format;

	if (stat(map->argv[0], &st) == -1) {
		warn(ap->logopt, "file map %s not found", map->argv[0]);
		return NSS_STATUS_NOTFOUND;
	}

	if (!S_ISREG(st.st_mode))
		return NSS_STATUS_NOTFOUND;

	if (st.st_mode & __S_IEXEC)
		type = src_prog;
	else
		type = src_file;

	format = map->format;

	instance = master_find_source_instance(map, type, format, 0, NULL);
	if (!instance) {
		int argc = map->argc;
		const char **argv = map->argv;
		instance = master_add_source_instance(map, type, format, age, argc, argv);
		if (!instance)
			return NSS_STATUS_UNAVAIL;
		instance->recurse = map->recurse;
		instance->depth = map->depth;
	}
	instance->stale = map->stale;

	return do_read_map(ap, instance, age);
}