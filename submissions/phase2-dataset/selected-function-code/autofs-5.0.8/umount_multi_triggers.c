int umount_multi_triggers(struct autofs_point *ap, struct mapent *me, char *root, const char *base)
{
	char path[PATH_MAX + 1];
	char *offset;
	struct mapent *oe;
	struct list_head *mm_root, *pos;
	const char o_root[] = "/";
	const char *mm_base;
	int left, start;

	left = 0;
	start = strlen(root);

	mm_root = &me->multi->multi_list;

	if (!base)
		mm_base = o_root;
	else
		mm_base = base;

	pos = NULL;
	offset = path;

	while ((offset = cache_get_offset(mm_base, offset, start, mm_root, &pos))) {
		char *oe_base;

		oe = cache_lookup_offset(mm_base, offset, start, &me->multi_list);
		/* root offset is a special case */
		if (!oe || !oe->mapent || (strlen(oe->key) - start) == 1)
			continue;

		/*
		 * Check for and umount subtree offsets resulting from
		 * nonstrict mount fail.
		 */
		oe_base = oe->key + strlen(root);
		left += umount_multi_triggers(ap, oe, root, oe_base);

		if (oe->ioctlfd != -1 ||
		    is_mounted(_PROC_MOUNTS, oe->key, MNTS_REAL)) {
			left++;
			continue;
		}

		debug(ap->logopt, "umount offset %s", oe->key);

		if (umount_autofs_offset(ap, oe)) {
			warn(ap->logopt, "failed to umount offset");
			left++;
		} else {
			struct stat st;
			int ret;

			if (!(oe->flags & MOUNT_FLAG_DIR_CREATED))
				continue;

			/*
			 * An error due to partial directory removal is
			 * ok so only try and remount the offset if the
			 * actual mount point still exists.
			 */
			ret = rmdir_path_offset(ap, oe);
			if (ret == -1 && !stat(oe->key, &st)) {
				ret = do_mount_autofs_offset(ap, oe, root, offset);
				if (ret)
					left++;
				/* But we did origianlly create this */
				oe->flags |= MOUNT_FLAG_DIR_CREATED;
			}
		}
	}

	if (!left && me->multi == me) {
		struct mapent_cache *mc = me->mc;
		int status;

		/*
		 * Special case.
		 * If we can't umount the root container then we can't
		 * delete the offsets from the cache and we need to put
		 * the offset triggers back.
		 */
		if (is_mounted(_PATH_MOUNTED, root, MNTS_REAL)) {
			info(ap->logopt, "unmounting dir = %s", root);
			if (umount_ent(ap, root)) {
				if (mount_multi_triggers(ap, me, root, strlen(root), "/") < 0)
					warn(ap->logopt,
					     "failed to remount offset triggers");
				return left++;
			}
		}

		/* We're done - clean out the offsets */
		status = cache_delete_offset_list(mc, me->key);
		if (status != CHE_OK)
			warn(ap->logopt, "couldn't delete offset list");
	}

	return left;
}