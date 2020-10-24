static int umount_subtree_mounts(struct autofs_point *ap, const char *path, unsigned int is_autofs_fs)
{
	struct mapent_cache *mc;
	struct mapent *me;
	unsigned int is_mm_root;
	int left;

	me = lookup_source_mapent(ap, path, LKP_DISTINCT);
	if (!me) {
		char *ind_key;

		ind_key = strrchr(path, '/');
		if (ind_key)
			ind_key++;

		me = lookup_source_mapent(ap, ind_key, LKP_NORMAL);
		if (!me)
			return 0;
	}

	mc = me->mc;
	is_mm_root = (me->multi == me);

	left = 0;

	pthread_cleanup_push(cache_lock_cleanup, mc);

	if (me->multi) {
		char root[PATH_MAX];
		char *base;
		int cur_state;

		if (!strchr(me->multi->key, '/'))
			/* Indirect multi-mount root */
			/* sprintf okay - if it's mounted, it's
			 * PATH_MAX or less bytes */
			sprintf(root, "%s/%s", ap->path, me->multi->key);
		else
			strcpy(root, me->multi->key);

		if (is_mm_root)
			base = NULL;
		else
			base = me->key + strlen(root);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
		/* Lock the closest parent nesting point for umount */
		cache_multi_writelock(me->parent);
		if (umount_multi_triggers(ap, me, root, base)) {
			warn(ap->logopt,
			     "some offset mounts still present under %s", path);
			left++;
		}
		cache_multi_unlock(me->parent);
		pthread_setcancelstate(cur_state, NULL);
	}

	pthread_cleanup_pop(1);

	if (left || is_autofs_fs)
		return left;

	/*
	 * If this is the root of a multi-mount we've had to umount
	 * it already to ensure it's ok to remove any offset triggers.
	 */
	if (!is_mm_root && is_mounted(_PATH_MOUNTED, path, MNTS_REAL)) {
		debug(ap->logopt, "unmounting dir = %s", path);
		if (umount_ent(ap, path)) {
			warn(ap->logopt, "could not umount dir %s", path);
			left++;
		}
	}

	return left;
}