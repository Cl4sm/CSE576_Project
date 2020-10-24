int master_mount_mounts(struct master *master, time_t age, int readall)
{
	struct mapent_cache *nc = master->nc;
	struct list_head *p, *head;
	int cur_state;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	master_mutex_lock();

	head = &master->mounts;
	p = head->next;
	while (p != head) {
		struct master_mapent *this;
		struct autofs_point *ap;
		struct mapent *ne, *nested;
		struct stat st;
		int state_pipe, save_errno;
		int ret;

		this = list_entry(p, struct master_mapent, list);
		p = p->next;

		ap = this->ap;

		/* A master map entry has gone away */
		if (this->age < age) {
			st_add_task(ap, ST_SHUTDOWN_PENDING);
			continue;
		}

		cache_readlock(nc);
		ne = cache_lookup_distinct(nc, this->path);
		/*
		 * If this path matched a nulled entry the master map entry
		 * must be an indirect mount so the master map entry line
		 * number may be obtained from this->maps.
		 */
		if (ne) {
			int lineno = ne->age;
			cache_unlock(nc);

			/* null entry appears after map entry */
			if (this->maps->master_line < lineno) {
				warn(ap->logopt,
				     "ignoring null entry that appears after "
				     "existing entry for %s", this->path);
				goto cont;
			}
			if (ap->state != ST_INIT) {
				st_add_task(ap, ST_SHUTDOWN_PENDING);
				continue;
			}
			/*
			 * The map entry hasn't been started yet and we've
			 * seen a preceeding null map entry for it so just
			 * delete it from the master map entry list so it
			 * doesn't get in the road.
			 */
			list_del_init(&this->list);
			master_free_mapent_sources(ap->entry, 1);
			master_free_mapent(ap->entry);
			continue;
		}
		nested = cache_partial_match(nc, this->path);
		if (nested) {
			error(ap->logopt,
			     "removing invalid nested null entry %s",
			     nested->key);
			nested = cache_partial_match(nc, this->path);
			if (nested)
				cache_delete(nc, nested->key);
		}
		cache_unlock(nc);
cont:
		st_mutex_lock();

		state_pipe = this->ap->state_pipe[1];

		/* No pipe so mount is needed */
		ret = fstat(state_pipe, &st);
		save_errno = errno;

		st_mutex_unlock();

		if (!ret)
			check_update_map_sources(this, readall);
		else if (ret == -1 && save_errno == EBADF) {
			if (!master_do_mount(this)) {
				list_del_init(&this->list);
				master_free_mapent_sources(ap->entry, 1);
				master_free_mapent(ap->entry);
			}
		}
	}

	master_mutex_unlock();
	pthread_setcancelstate(cur_state, NULL);

	return 1;
}