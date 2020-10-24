void *expire_proc_direct(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct mnt_list *mnts = NULL, *next;
	struct list_head list, *p;
	struct expire_args *ea;
	struct expire_args ec;
	struct autofs_point *ap;
	struct mapent *me = NULL;
	unsigned int now;
	int ioctlfd, cur_state;
	int status, ret, left;

	ea = (struct expire_args *) arg;

	status = pthread_mutex_lock(&ea->mutex);
	if (status)
		fatal(status);

	ap = ec.ap = ea->ap;
	now = ea->when;
	ec.status = -1;

	ea->signaled = 1;
	status = pthread_cond_signal(&ea->cond);
	if (status)
		fatal(status);

	status = pthread_mutex_unlock(&ea->mutex);
	if (status)
		fatal(status);

	pthread_cleanup_push(expire_cleanup, &ec);

	left = 0;

	mnts = tree_make_mnt_tree(_PROC_MOUNTS, "/");
	pthread_cleanup_push(mnts_cleanup, mnts);

	/* Get a list of mounts select real ones and expire them if possible */
	INIT_LIST_HEAD(&list);
	if (!tree_get_mnt_list(mnts, &list, "/", 0)) {
		ec.status = 0;
		return NULL;
	}

	list_for_each(p, &list) {
		next = list_entry(p, struct mnt_list, list);

		/*
		 * All direct mounts must be present in the map
		 * entry cache.
		 */
		pthread_cleanup_push(master_source_lock_cleanup, ap->entry);
		master_source_readlock(ap->entry);
		me = lookup_source_mapent(ap, next->path, LKP_DISTINCT);
		pthread_cleanup_pop(1);
		if (!me)
			continue;

		if (!strcmp(next->fs_type, "autofs")) {
			struct stat st;
			int ioctlfd;

			cache_unlock(me->mc);

			/*
			 * If we have submounts check if this path lives below
			 * one of them and pass on state change.
			 */
			pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
			if (strstr(next->opts, "indirect")) {
				master_notify_submount(ap, next->path, ap->state);
				pthread_setcancelstate(cur_state, NULL);
				continue;
			}

			if (me->ioctlfd == -1) {
				pthread_setcancelstate(cur_state, NULL);
				continue;
			}

			/* It's got a mount, deal with in the outer loop */
			if (tree_is_mounted(mnts, me->key, MNTS_REAL)) {
				pthread_setcancelstate(cur_state, NULL);
				continue;
			}

			/*
			 * Maybe a manual umount, repair.
			 * It will take ap->exp_timeout/4 for us to relaize
			 * this so user must still use USR1 signal to close
			 * the open file handle for mounts atop multi-mount
			 * triggers. There is no way that I'm aware of to
			 * avoid maintaining a file handle for control
			 * functions as once it's mounted all opens are
			 * directed to the mount not the trigger.
			 */

			/* Check for manual umount */
			cache_writelock(me->mc);
			if (me->ioctlfd != -1 && 
			    fstat(me->ioctlfd, &st) != -1 &&
			    !count_mounts(ap->logopt, next->path, st.st_dev)) {
				ops->close(ap->logopt, me->ioctlfd);
				me->ioctlfd = -1;
				cache_unlock(me->mc);
				pthread_setcancelstate(cur_state, NULL);
				continue;
			}
			cache_unlock(me->mc);

			ioctlfd = me->ioctlfd;

			ret = ops->expire(ap->logopt, ioctlfd, next->path, now);
			if (ret) {
				left++;
				pthread_setcancelstate(cur_state, NULL);
				continue;
			}

			pthread_setcancelstate(cur_state, NULL);
			continue;
		}

		if (me->ioctlfd >= 0) {
			/* Real mounts have an open ioctl fd */
			ioctlfd = me->ioctlfd;
			cache_unlock(me->mc);
		} else {
			cache_unlock(me->mc);
			continue;
		}

		if (ap->state == ST_EXPIRE || ap->state == ST_PRUNE)
			pthread_testcancel();

		debug(ap->logopt, "send expire to trigger %s", next->path);

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
		ret = ops->expire(ap->logopt, ioctlfd, next->path, now);
		if (ret)
			left++;
		pthread_setcancelstate(cur_state, NULL);
	}
	pthread_cleanup_pop(1);

	if (left)
		info(ap->logopt, "%d remaining in %s", left, ap->path);

	ec.status = left;

	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cur_state);
	pthread_cleanup_pop(1);
	pthread_setcancelstate(cur_state, NULL);

	return NULL;
}