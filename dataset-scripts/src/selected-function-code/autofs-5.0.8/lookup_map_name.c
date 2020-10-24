static enum nsswitch_status lookup_map_name(struct nss_source *this,
			struct autofs_point *ap, struct map_source *map,
			const char *name, int name_len)
{
	enum nsswitch_status result;
	struct map_source tmap;
	char *path;

	if (strcasecmp(this->source, "files"))
		return lookup_name_source_instance(ap, map,
					this->source, name, name_len);

	/* 
	 * autofs build-in map for nsswitch "files" is "file".
	 * This is a special case as we need to append the
	 * normal location to the map name.
	 * note: we consider it invalid to specify a relative
	 *       path.
	 */
	if (strchr(map->argv[0], '/')) {
		error(ap->logopt, "relative path invalid in files map name");
		return NSS_STATUS_NOTFOUND;
	}

	this->source[4] = '\0';
	tmap.type = this->source;
	tmap.format = map->format;
	tmap.mc = map->mc;
	tmap.instance = map->instance;
	tmap.exp_timeout = map->exp_timeout;
	tmap.recurse = map->recurse;
	tmap.depth = map->depth;
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

	result = lookup_name_file_source_instance(ap, &tmap, name, name_len);

	map->instance = tmap.instance;

	/* path is freed in free_argv */
	free_argv(tmap.argc, tmap.argv);

	return result;
}