int master_notify_submount(struct autofs_point *ap, const char *path, enum states state)
{
	struct list_head *head, *p;
	struct autofs_point *this = NULL;
	int ret = 1;

	mounts_mutex_lock(ap);

	head = &ap->submounts;
	p = head->prev;
	while (p != head) {
		this = list_entry(p, struct autofs_point, mounts);
		p = p->prev;

		/* path not the same */
		if (strcmp(this->path, path))
			continue;

		if (!master_submount_list_empty(this)) {
			char *this_path = strdup(this->path);
			if (this_path) {
				mounts_mutex_unlock(ap);
				master_notify_submount(this, path, state);
				mounts_mutex_lock(ap);
				if (!__master_find_submount(ap, this_path)) {
					free(this_path);
					continue;
				}
				free(this_path);
			}
		}

		/* Now we have found the submount we want to expire */

		st_mutex_lock();

		if (this->state == ST_SHUTDOWN) {
			this = NULL;
			st_mutex_unlock();
			break;
		}

		this->shutdown = ap->shutdown;

		__st_add_task(this, state);

		st_mutex_unlock();
		mounts_mutex_unlock(ap);

		st_wait_task(this, state, 0);

		/*
		 * If our submount gets to state ST_SHUTDOWN, ST_SHUTDOWN_PENDING or
		 * ST_SHUTDOWN_FORCE we need to wait until it goes away or changes
		 * to ST_READY.
		 */
		mounts_mutex_lock(ap);
		st_mutex_lock();
		while ((this = __master_find_submount(ap, path))) {
			struct timespec t = { 0, 300000000 };
			struct timespec r;

			if (this->state != ST_SHUTDOWN &&
			    this->state != ST_SHUTDOWN_PENDING &&
			    this->state != ST_SHUTDOWN_FORCE) {
				ret = 0;
				break;
			}

			st_mutex_unlock();
			mounts_mutex_unlock(ap);
			while (nanosleep(&t, &r) == -1 && errno == EINTR)
				memcpy(&t, &r, sizeof(struct timespec));
			mounts_mutex_lock(ap);
			st_mutex_lock();
		}
		st_mutex_unlock();
		break;
	}

	mounts_mutex_unlock(ap);

	return ret;
}