void master_source_readlock(struct master_mapent *entry)
{
	int retries = 25;
	int status;

	while (retries--) {
		status = pthread_rwlock_rdlock(&entry->source_lock);
		if (status != EAGAIN && status != EBUSY)
			break;
		else {
                	struct timespec t = { 0, 200000000 };
	                struct timespec r;

			if (status == EAGAIN)
				logmsg("master_mapent source too many readers");
			else
				logmsg("master_mapent source write lock held");

                	while (nanosleep(&t, &r) == -1 && errno == EINTR)
                        	memcpy(&t, &r, sizeof(struct timespec));
		}
	}

	if (status) {
		logmsg("master_mapent source read lock failed");
		fatal(status);
	}

	return;
}