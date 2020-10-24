void lookup_prune_one_cache(struct autofs_point *ap, struct mapent_cache *mc, time_t age)
{
	struct mapent *me, *this;
	char *path;
	int status = CHE_FAIL;

	me = cache_enumerate(mc, NULL);
	while (me) {
		struct mapent *valid;
		char *key = NULL, *next_key = NULL;

		if (me->age >= age) {
			/*
			 * Reset time of last fail for valid map entries to
			 * force entry update and subsequent mount retry.
			 */
			me->status = 0;
			me = cache_enumerate(mc, me);
			continue;
		}

		key = strdup(me->key);
		me = cache_enumerate(mc, me);
		if (!key || !strcmp(key, "*")) {
			if (key)
				free(key);
			continue;
		}

		path = make_fullpath(ap->path, key);
		if (!path) {
			warn(ap->logopt, "can't malloc storage for path");
			free(key);
			continue;
		}

		/*
		 * If this key has another valid entry we want to prune it,
		 * even if it's a mount, as the valid entry will take the
		 * mount if it is a direct mount or it's just a stale indirect
		 * cache entry.
		 */
		valid = lookup_source_valid_mapent(ap, key, LKP_DISTINCT);
		if (valid && valid->mc == mc) {
			 /*
			  * We've found a map entry that has been removed from
			  * the current cache so it isn't really valid.
			  */
			cache_unlock(valid->mc);
			valid = NULL;
		}
		if (!valid &&
		    is_mounted(_PATH_MOUNTED, path, MNTS_REAL)) {
			debug(ap->logopt,
			      "prune check posponed, %s mounted", path);
			free(key);
			free(path);
			continue;
		}
		if (valid)
			cache_unlock(valid->mc);

		if (me)
			next_key = strdup(me->key);

		cache_unlock(mc);

		cache_writelock(mc);
		this = cache_lookup_distinct(mc, key);
		if (!this) {
			cache_unlock(mc);
			goto next;
		}

		if (valid)
			cache_delete(mc, key);
		else if (!is_mounted(_PROC_MOUNTS, path, MNTS_AUTOFS)) {
			dev_t devid = ap->dev;
			status = CHE_FAIL;
			if (ap->type == LKP_DIRECT)
				devid = this->dev;
			if (this->ioctlfd == -1)
				status = cache_delete(mc, key);
			if (status != CHE_FAIL) {
				if (ap->type == LKP_INDIRECT) {
					if (ap->flags & MOUNT_FLAG_GHOST)
						rmdir_path(ap, path, devid);
				} else
					rmdir_path(ap, path, devid);
			}
		}
		cache_unlock(mc);

next:
		cache_readlock(mc);
		if (next_key) {
			me = cache_lookup_distinct(mc, next_key);
			free(next_key);
		}
		free(key);
		free(path);
	}

	return;
}