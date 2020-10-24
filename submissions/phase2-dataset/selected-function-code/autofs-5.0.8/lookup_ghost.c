int lookup_ghost(struct autofs_point *ap, const char *root)
{
	struct master_mapent *entry = ap->entry;
	struct map_source *map;
	struct mapent_cache *mc;
	struct mapent *me;
	char buf[MAX_ERR_BUF];
	struct stat st;
	char *fullpath;
	int ret;

	if (!strcmp(ap->path, "/-"))
		return LKP_FAIL | LKP_DIRECT;

	if (!(ap->flags & MOUNT_FLAG_GHOST))
		return LKP_INDIRECT;

	pthread_cleanup_push(master_source_lock_cleanup, entry);
	master_source_readlock(entry);
	map = entry->maps;
	while (map) {
		/*
		 * Only consider map sources that have been read since 
		 * the map entry was last updated.
		 */
		if (entry->age > map->age) {
			map = map->next;
			continue;
		}

		mc = map->mc;
		pthread_cleanup_push(cache_lock_cleanup, mc);
		cache_readlock(mc);
		me = cache_enumerate(mc, NULL);
		while (me) {
			if (!strcmp(me->key, "*"))
				goto next;

			if (*me->key == '/') {
				/* It's a busy multi-mount - leave till next time */
				if (list_empty(&me->multi_list))
					error(ap->logopt,
					      "invalid key %s", me->key);
				goto next;
			}

			fullpath = malloc(strlen(me->key) + strlen(root) + 3);
			if (!fullpath) {
				warn(ap->logopt, "failed to allocate full path");
				goto next;
			}
			sprintf(fullpath, "%s/%s", root, me->key);

			ret = stat(fullpath, &st);
			if (ret == -1 && errno != ENOENT) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				warn(ap->logopt, "stat error %s", estr);
				free(fullpath);
				goto next;
			}

			ret = mkdir_path(fullpath, 0555);
			if (ret < 0 && errno != EEXIST) {
				char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
				warn(ap->logopt,
				     "mkdir_path %s failed: %s", fullpath, estr);
				free(fullpath);
				goto next;
			}

			if (stat(fullpath, &st) != -1) {
				me->dev = st.st_dev;
				me->ino = st.st_ino;
			}

			free(fullpath);
next:
			me = cache_enumerate(mc, me);
		}
		pthread_cleanup_pop(1);
		map = map->next;
	}
	pthread_cleanup_pop(1);

	return LKP_INDIRECT;
}