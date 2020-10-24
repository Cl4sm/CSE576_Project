static int lookup_name_source_instance(struct autofs_point *ap, struct map_source *map, const char *type, const char *name, int name_len)
{
	struct map_source *instance;
	const char *format;
	time_t age = time(NULL);

	format = map->format;

	instance = master_find_source_instance(map, type, format, 0, NULL);
	if (!instance) {
		int argc = map->argc;
		const char **argv = map->argv;
		instance = master_add_source_instance(map, type, format, age, argc, argv);
		if (!instance)
			return NSS_STATUS_NOTFOUND;
		instance->recurse = map->recurse;
		instance->depth = map->depth;
	}

	return do_lookup_mount(ap, instance, name, name_len);
}