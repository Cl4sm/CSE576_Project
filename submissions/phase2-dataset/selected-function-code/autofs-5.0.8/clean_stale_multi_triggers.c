int clean_stale_multi_triggers(struct autofs_point *ap,
			       struct mapent *me, char *top, const char *base)
{
	char *root;
	char mm_top[PATH_MAX + 1];
	char path[PATH_MAX + 1];
	char buf[MAX_ERR_BUF];
	char *offset;
	struct mapent *oe;
	struct list_head *mm_root, *pos;
	const char o_root[] = "/";
	const char *mm_base;
	int left, start;
	time_t age;

	if (top)
		root = top;
	else {
		if (!strchr(me->multi->key, '/'))
			/* Indirect multi-mount root */
			/* sprintf okay - if it's mounted, it's
			 * PATH_MAX or less bytes */
			sprintf(mm_top, "%s/%s", ap->path, me->multi->key);
		else
			strcpy(mm_top, me->multi->key);
		root = mm_top;
	}

	left = 0;
	start = strlen(root);

	mm_root = &me->multi->multi_list;

	if (!base)
		mm_base = o_root;
	else
		mm_base = base;

	pos = NULL;
	offset = path;
	age = me->multi->age;

	while ((offset = cache_get_offset(mm_base, offset, start, mm_root, &pos))) {
		char *oe_base;
		char *key;
		int ret;

		oe = cache_lookup_offset(mm_base, offset, start, &me->multi_list);
		/* root offset is a special case */
		if (!oe || !oe->mapent || (strlen(oe->key) - start) == 1)
			continue;

		/* Check for and umount stale subtree offsets */
		oe_base = oe->key + strlen(root);
		ret = clean_stale_multi_triggers(ap, oe, root, oe_base);
		left =+ ret;
		if (ret)
			continue;

		if (oe->age == age)
			continue;

		/*
		 * If an offset that has an active mount has been removed
		 * from the multi-mount we don't want to attempt to trigger
		 * mounts for it. Obviously this is because it has been
		 * removed, but less obvious is the potential strange
		 * behaviour that can result if we do try and mount it
		 * again after it's been expired. For example, if an NFS
		 * file system is no longer exported and is later umounted
		 * it can be mounted again without any error message but
		 * shows as an empty directory. That's going to confuse
		 * people for sure.
		 *
		 * If the mount cannot be umounted (the process is now
		 * using a stale mount) the offset needs to be invalidated
		 * so no further mounts will be attempted but the offset
		 * cache entry must remain so expires can continue to
		 * attempt to umount it. If the mount can be umounted and
		 * the offset is removed, at least for NFS we will get
		 * ESTALE errors when attempting list the directory.
		 */
		if (oe->ioctlfd != -1 ||
		    is_mounted(_PROC_MOUNTS, oe->key, MNTS_REAL)) {
			if (umount_ent(ap, oe->key)) {
				debug(ap->logopt,
				      "offset %s has active mount, invalidate",
				      oe->key);
				if (oe->mapent) {
					free(oe->mapent);
					oe->mapent = NULL;
				}
				left++;
				continue;
			}
		}

		key = strdup(oe->key);
		if (!key) {
	                char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
        	        error(ap->logopt, "malloc: %s", estr);
			left++;
			continue;
		}

		debug(ap->logopt, "umount offset %s", oe->key);

		if (umount_autofs_offset(ap, oe)) {
			warn(ap->logopt, "failed to umount offset %s", key);
			left++;
		} else {
			struct stat st;

			/* Mount point not ours to delete ? */
			if (!(oe->flags & MOUNT_FLAG_DIR_CREATED)) {
				debug(ap->logopt, "delete offset key %s", key);
				if (cache_delete_offset(oe->mc, key) == CHE_FAIL)
					error(ap->logopt,
					     "failed to delete offset key %s", key);
				free(key);
				continue;
			}

			/*
			 * An error due to partial directory removal is
			 * ok so only try and remount the offset if the
			 * actual mount point still exists.
			 */
			ret = rmdir_path_offset(ap, oe);
			if (ret == -1 && !stat(oe->key, &st)) {
				ret = do_mount_autofs_offset(ap, oe, root, offset);
				if (ret) {
					left++;
					/* But we did origianlly create this */
					oe->flags |= MOUNT_FLAG_DIR_CREATED;
					free(key);
					continue;
				}
				/*
				 * Fall through if the trigger can't be mounted
				 * again, since there is no offset there can't
				 * be any mount requests so remove the map
				 * entry from the cache. There's now a dead
				 * offset mount, but what else can we do ....
				 */
			}

			debug(ap->logopt, "delete offset key %s", key);

			if (cache_delete_offset(oe->mc, key) == CHE_FAIL)
				error(ap->logopt,
				     "failed to delete offset key %s", key);
		}
		free(key);
	}

	return left;
}