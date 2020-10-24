static struct map_source *
__master_find_map_source(struct master_mapent *entry,
			 const char *type, const char *format,
			 int argc, const char **argv)
{
	struct map_source *map;
	struct map_source *source = NULL;
	int res;

	map = entry->maps;
	while (map) {
		res = compare_source_type_and_format(map, type, format);
		if (!res)
			goto next;

		res = compare_argv(map->argc, map->argv, argc, argv);
		if (!res)
			goto next;

		source = map;
		break;
next:
		map = map->next;
	}

	return source;
}