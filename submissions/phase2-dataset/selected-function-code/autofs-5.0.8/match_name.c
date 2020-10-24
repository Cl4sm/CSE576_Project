static int match_name(struct map_source *source, const char *name)
{
	int argc = source->argc;
	int ret = 0;
	int i;

	/*
	 * This can't work for old style "multi" type sources since
	 * there's no way to know from which map the cache entry came
	 * from and duplicate entries are ignored at map read time.
	 * All we can really do is list all the entries for the given
	 * multi map if one of its map names matches.
	 */
	for (i = 0; i < argc; i++) {
		if (i == 0 || !strcmp(source->argv[i], "--")) {
			if (i != 0) {
				i++;
				if (i >= argc)
					break;
			}

			if (source->argv[i] && *source->argv[i] != '-') {
				char *map = get_map_name(source->argv[i]);
				if (!map)
					break;
				if (!strcmp(map, name)) {
					ret = 1;
					free(map);
					break;
				}
				free(map);
			}
		}
	}

	return ret;
}