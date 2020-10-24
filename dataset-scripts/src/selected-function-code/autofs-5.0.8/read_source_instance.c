static int read_source_instance(struct autofs_point *ap, struct map_source *map, const char *type, time_t age)
{
	struct map_source *instance;
	const char *format;

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