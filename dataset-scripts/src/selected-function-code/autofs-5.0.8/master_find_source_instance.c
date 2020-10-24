struct map_source *master_find_source_instance(struct map_source *source, const char *type, const char *format, int argc, const char **argv)
{
	struct map_source *map;
	struct map_source *instance = NULL;
	int status, res;

	status = pthread_mutex_lock(&instance_mutex);
	if (status)
		fatal(status);

	map = source->instance;
	while (map) {
		res = compare_source_type_and_format(map, type, format);
		if (!res)
			goto next;

		if (!argv) {
			instance = map;
			break;
		}

		res = compare_argv(map->argc, map->argv, argc, argv);
		if (!res)
			goto next;

		instance = map;
		break;
next:
		map = map->next;
	}

	status = pthread_mutex_unlock(&instance_mutex);
	if (status)
		fatal(status);

	return instance;
}