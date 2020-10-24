static void do_readmap_mount(struct autofs_point *ap, struct mnt_list *mnts,
			     struct map_source *map, struct mapent *me, time_t now)
{
	struct mapent_cache *nc;
	struct mapent *ne, *nested, *valid;

	nc = ap->entry->master->nc;

	ne = cache_lookup_distinct(nc, me->key);
	if (!ne) {
		nested = cache_partial_match(nc, me->key);
		if (nested) {
			error(ap->logopt,
			      "removing invalid nested null entry %s",
			      nested->key);
			nested = cache_partial_match(nc, me->key);
			if (nested)
				cache_delete(nc, nested->key);
		}
	}

	if (me->age < now || (ne && map->master_line > ne->age)) {
		/*
		 * The map instance may have changed, such as the map name or
		 * the mount options, but the direct map entry may still exist
		 * in one of the other maps. If so then update the new cache
		 * entry device and inode so we can find it at lookup. Later,
		 * the mount for the new cache entry will just update the
		 * timeout.
		 *
		 * TODO: how do we recognise these orphaned map instances. We
		 * can't just delete these instances when the cache becomes
		 * empty because that is a valid state for a master map entry.
		 * This is becuase of the requirement to continue running with
		 * an empty cache awaiting a map re-load.
		 */
		valid = lookup_source_valid_mapent(ap, me->key, LKP_DISTINCT);
		if (valid && valid->mc == me->mc) {
			/*
			 * We've found a map entry that has been removed from
			 * the current cache so there is no need to update it.
			 * The stale entry will be dealt with when we prune the
			 * cache later.
			 */
			cache_unlock(valid->mc);
			valid = NULL;
		}
		if (valid) {
			struct mapent_cache *vmc = valid->mc;
			struct ioctl_ops *ops = get_ioctl_ops();
			time_t runfreq;

			cache_unlock(vmc);
			debug(ap->logopt,
			     "updating cache entry for valid direct trigger %s",
			     me->key);
			cache_writelock(vmc);
			valid = cache_lookup_distinct(vmc, me->key);
			if (!valid) {
				cache_unlock(vmc);
				error(ap->logopt,
				     "failed to find expected existing valid map entry");
				return;
			}
			/* Take over the mount if there is one */
			valid->ioctlfd = me->ioctlfd;
			me->ioctlfd = -1;
			/* Set device and inode number of the new mapent */
			cache_set_ino_index(vmc, me->key, me->dev, me->ino);
			cache_unlock(vmc);
			/* Set timeout and calculate the expire run frequency */
			ops->timeout(ap->logopt, valid->ioctlfd, map->exp_timeout);
			if (map->exp_timeout) {
				runfreq = (map->exp_timeout + CHECK_RATIO - 1) / CHECK_RATIO;
				if (ap->exp_runfreq)
					ap->exp_runfreq = min(ap->exp_runfreq, runfreq);
				else
					ap->exp_runfreq = runfreq;
			}
		} else if (!tree_is_mounted(mnts, me->key, MNTS_REAL))
			do_umount_autofs_direct(ap, mnts, me);
		else
			debug(ap->logopt,
			      "%s is mounted", me->key);
	} else
		do_mount_autofs_direct(ap, mnts, me, map->exp_timeout);

	return;
}