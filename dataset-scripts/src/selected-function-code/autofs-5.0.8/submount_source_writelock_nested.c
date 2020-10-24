static int submount_source_writelock_nested(struct autofs_point *ap)
{
	struct autofs_point *parent = ap->parent;
	int status;

	status = pthread_rwlock_trywrlock(&parent->entry->source_lock);
	if (status)
		goto done;

	mounts_mutex_lock(parent);

	status = pthread_rwlock_trywrlock(&ap->entry->source_lock);
	if (status) {
		mounts_mutex_unlock(parent);
		master_source_unlock(parent->entry);
	}

done:
	if (status && status != EBUSY) {
		logmsg("submount nested master_mapent source write lock failed");
		fatal(status);
	}

	return status;
}