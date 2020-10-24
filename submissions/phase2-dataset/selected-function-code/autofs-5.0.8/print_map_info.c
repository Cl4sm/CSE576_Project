static void print_map_info(struct map_source *source)
{
	int argc = source->argc;
	int i, multi, map_num;

	multi = (source->type && !strcmp(source->type, "multi"));
	map_num = 1;
	for (i = 0; i < argc; i++) {
		if (source->argv[i] && *source->argv[i] != '-') {
			if (!multi)
				printf("  map: %s\n", source->argv[i]);
			else
				printf("  map[%i]: %s\n", map_num, source->argv[i]);
			i++;
		}

		if (i >= argc)
			return;

		if (!strcmp(source->argv[i], "--"))
			continue;

		if (source->argv[i]) {
			int need_newline = 0;
			int j;

			if (!multi)
				printf("  arguments:");
			else
				printf("  arguments[%i]:", map_num);

			for (j = i; j < source->argc; j++) {
				if (!strcmp(source->argv[j], "--"))
					break;
				printf(" %s", source->argv[j]);
				i++;
				need_newline = 1;
			}
			if (need_newline)
				printf("\n");
		}
		if (multi)
			map_num++;
	}

	return;
}