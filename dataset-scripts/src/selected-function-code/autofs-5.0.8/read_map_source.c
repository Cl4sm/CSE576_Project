static enum nsswitch_status read_map_source(struct nss_source *this,
		struct autofs_point *ap, struct map_source *map, time_t age)
{
	enum nsswitch_status result;
	struct map_source tmap;
	char *path;

	if (strcasecmp(this->source, "files")) {
		return read_source_instance(ap, map, this->source, age);
	}

	/* 
	 * autofs built-in map for nsswitch "files" is "file".
	 * This is a special case as we need to append the
	 * normal location to the map name.
	 * note: It's invalid to specify a relative path.
	 */

	if (strchr(map->argv[0], '/')) {
		error(ap->logopt, "relative path invalid in files map name");
		return NSS_STATUS_NOTFOUND;
	}

	this->source[4] = '\0';
	tmap.type = this->source;
	tmap.format = map->format;
	tmap.lookup = map->lookup;
	tmap.mc = map->mc;
	tmap.instance = map->instance;
	tmap.exp_timeout = map->exp_timeout;
	tmap.recurse = map->recurse;
	tmap.depth = map->depth;
	tmap.stale = map->stale;
	tmap.argc = 0;
	tmap.argv = NULL;

	path = malloc(strlen(AUTOFS_MAP_DIR) + strlen(map->argv[0]) + 2);
	if (!path)
		return NSS_STATUS_UNKNOWN;

	strcpy(path, AUTOFS_MAP_DIR);
	strcat(path, "/");
	strcat(path, map->argv[0]);

	if (map->argc >= 1) {
		tmap.argc = map->argc;
		tmap.argv = copy_argv(map->argc, map->argv);
		if (!tmap.argv) {
			error(ap->logopt, "failed to copy args");
			free(path);
			return NSS_STATUS_UNKNOWN;
		}
		if (tmap.argv[0])
			free((char *) tmap.argv[0]);
		tmap.argv[0] = path;
	} else {
		error(ap->logopt, "invalid arguments for autofs_point");
		free(path);
		return NSS_STATUS_UNKNOWN;
	}

	pthread_cleanup_push(argv_cleanup, &tmap);
	result = read_file_source_instance(ap, &tmap, age);
	pthread_cleanup_pop(1);

	map->instance = tmap.instance;

	return result;
}