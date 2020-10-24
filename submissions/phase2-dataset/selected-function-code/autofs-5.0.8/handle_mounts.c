void *handle_mounts(void *arg)
{
	struct startup_cond *suc;
	struct autofs_point *ap;
	int cancel_state, status = 0;
	char *root;

	suc = (struct startup_cond *) arg;

	ap = suc->ap;
	root = strdup(suc->root);

	pthread_cleanup_push(return_start_status, suc);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cancel_state);

	status = pthread_mutex_lock(&suc->mutex);
	if (status) {
		logerr("failed to lock startup condition mutex!");
		fatal(status);
	}

	if (!root) {
		crit(ap->logopt, "failed to alloc string root");
		suc->status = 1;
		pthread_setcancelstate(cancel_state, NULL);
		pthread_exit(NULL);
	}

	if (mount_autofs(ap, root) < 0) {
		crit(ap->logopt, "mount of %s failed!", ap->path);
		suc->status = 1;
		umount_autofs(ap, root, 1);
		free(root);
		pthread_setcancelstate(cancel_state, NULL);
		pthread_exit(NULL);
	}

	free(root);

	if (ap->flags & MOUNT_FLAG_NOBIND)
		info(ap->logopt, "bind mounts disabled");

	if (ap->flags & MOUNT_FLAG_GHOST && ap->type != LKP_DIRECT)
		info(ap->logopt, "ghosting enabled");

	suc->status = 0;
	pthread_cleanup_pop(1);

	/* We often start several automounters at the same time.  Add some
	   randomness so we don't all expire at the same time. */
	if (!ap->submount && ap->exp_runfreq)
		alarm_add(ap, ap->exp_runfreq + rand() % ap->exp_runfreq);

	pthread_setcancelstate(cancel_state, NULL);

	while (ap->state != ST_SHUTDOWN) {
		if (handle_packet(ap)) {
			int ret, cur_state;

			/*
			 * If we're a submount we need to ensure our parent
			 * doesn't try to mount us again until our shutdown
			 * is complete and that any outstanding mounts are
			 * completed before we try to shutdown.
			 */
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);

			master_mutex_lock();

			if (ap->submount) {
				/*
				 * If a mount request arrives before the locks are
				 * aquired just return to ready state.
				 */
				ret = submount_source_writelock_nested(ap);
				if (ret) {
					warn(ap->logopt,
					     "can't shutdown submount: mount in progress");
					/* Return to ST_READY is done immediately */
					st_add_task(ap, ST_READY);
					master_mutex_unlock();
					pthread_setcancelstate(cur_state, NULL);
					continue;
				}
			} else
				master_source_writelock(ap->entry);

			if (ap->state != ST_SHUTDOWN) {
				if (!ap->submount)
					alarm_add(ap, ap->exp_runfreq);
				/* Return to ST_READY is done immediately */
				st_add_task(ap, ST_READY);
				if (ap->submount)
					submount_source_unlock_nested(ap);
				else
					master_source_unlock(ap->entry);
				master_mutex_unlock();

				pthread_setcancelstate(cur_state, NULL);
				continue;
			}

			alarm_delete(ap);
			st_remove_tasks(ap);
			st_wait_task(ap, ST_ANY, 0);

			/*
			 * For a direct mount map all mounts have already gone
			 * by the time we get here and since we only ever
			 * umount direct mounts at shutdown there is no need
			 * to check for possible recovery.
			 */
			if (ap->type == LKP_DIRECT) {
				umount_autofs(ap, NULL, 1);
				break;
			}

			/*
			 * If umount_autofs returns non-zero it wasn't able
			 * to complete the umount and has left the mount intact
			 * so we can continue. This can happen if a lookup
			 * occurs while we're trying to umount.
			 */
			ret = umount_autofs(ap, NULL, 1);
			if (!ret)
				break;

			/* Failed shutdown returns to ready */
			warn(ap->logopt,
			     "can't shutdown: filesystem %s still busy",
			     ap->path);
			if (!ap->submount)
				alarm_add(ap, ap->exp_runfreq);
			/* Return to ST_READY is done immediately */
			st_add_task(ap, ST_READY);
			if (ap->submount)
				submount_source_unlock_nested(ap);
			else
				master_source_unlock(ap->entry);
			master_mutex_unlock();

			pthread_setcancelstate(cur_state, NULL);

		}
	}

	handle_mounts_cleanup(ap);

	return NULL;
}