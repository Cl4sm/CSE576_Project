int master_read_master(struct master *master, time_t age, int readall)
{
	unsigned int logopt = master->logopt;
	struct mapent_cache *nc;

	/*
	 * We need to clear and re-populate the null map entry cache
	 * before alowing anyone else to use it.
	 */
	master_mutex_lock();
	if (master->nc) {
		cache_writelock(master->nc);
		nc = master->nc;
		cache_clean_null_cache(nc);
	} else {
		nc = cache_init_null_cache(master);
		if (!nc) {
			error(logopt,
			      "failed to init null map cache for %s",
			      master->name);
			return 0;
		}
		cache_writelock(nc);
		master->nc = nc;
	}
	master_init_scan();
	lookup_nss_read_master(master, age);
	cache_unlock(nc);
	master_mutex_unlock();

	if (!master->read_fail)
		master_mount_mounts(master, age, readall);
	else {
		master->read_fail = 0;
		if (!readall)
			master_mount_mounts(master, age, readall);
	}

	master_mutex_lock();

	if (list_empty(&master->mounts))
		warn(logopt, "no mounts in table");

	master_mutex_unlock();

	return 1;
}