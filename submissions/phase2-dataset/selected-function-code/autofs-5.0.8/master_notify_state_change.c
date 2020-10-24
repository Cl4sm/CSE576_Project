void master_notify_state_change(struct master *master, int sig)
{
	struct master_mapent *entry;
	struct autofs_point *ap;
	struct list_head *p;
	int cur_state;
	unsigned int logopt;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	master_mutex_lock();

	list_for_each(p, &master->mounts) {
		enum states next = ST_INVAL;

		entry = list_entry(p, struct master_mapent, list);

		ap = entry->ap;
		logopt = ap->logopt;

		st_mutex_lock();

		if (ap->state == ST_SHUTDOWN)
			goto next;

		switch (sig) {
		case SIGTERM:
		case SIGINT:
			if (ap->state != ST_SHUTDOWN_PENDING &&
			    ap->state != ST_SHUTDOWN_FORCE) {
				next = ST_SHUTDOWN_PENDING;
				ap->shutdown = 1;
				__st_add_task(ap, next);
			}
			break;
#ifdef ENABLE_FORCED_SHUTDOWN
		case SIGUSR2:
			if (ap->state != ST_SHUTDOWN_FORCE &&
			    ap->state != ST_SHUTDOWN_PENDING) {
				next = ST_SHUTDOWN_FORCE;
				ap->shutdown = 1;
				__st_add_task(ap, next);
			}
			break;
#endif
		case SIGUSR1:
			assert(ap->state == ST_READY);
			next = ST_PRUNE;
			__st_add_task(ap, next);
			break;
		}
next:
		if (next != ST_INVAL)
			debug(logopt,
			      "sig %d switching %s from %d to %d",
			      sig, ap->path, ap->state, next);

		st_mutex_unlock();
	}

	master_mutex_unlock();
	pthread_setcancelstate(cur_state, NULL);

	return;
}