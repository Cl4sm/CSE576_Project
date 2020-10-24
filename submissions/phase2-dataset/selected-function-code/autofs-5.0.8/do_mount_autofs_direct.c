int do_mount_autofs_direct(struct autofs_point *ap,
			   struct mnt_list *mnts, struct mapent *me,
			   time_t timeout)
{
	const char *str_direct = mount_type_str(t_direct);
	struct ioctl_ops *ops = get_ioctl_ops();
	struct mnt_params *mp;
	struct stat st;
	int status, ret, ioctlfd;
	const char *map_name;
	time_t runfreq;

	if (timeout) {
		/* Calculate the expire run frequency */
		runfreq = (timeout + CHECK_RATIO - 1) / CHECK_RATIO;
		if (ap->exp_runfreq)
			ap->exp_runfreq = min(ap->exp_runfreq, runfreq);
		else
			ap->exp_runfreq = runfreq;
	}

	if (ops->version && !do_force_unlink) {
		ap->flags |= MOUNT_FLAG_REMOUNT;
		ret = try_remount(ap, me, t_direct);
		ap->flags &= ~MOUNT_FLAG_REMOUNT;
		if (ret == 1)
			return 0;
		if (ret == 0)
			return -1;
	} else {
		/*
		 * A return of 0 indicates we're re-reading the map.
		 * A return of 1 indicates we successfully unlinked
		 * the mount tree if there was one. A return of -1
		 * inducates we failed to unlink the mount tree so
		 * we have to return a failure.
		 */
		ret = unlink_active_mounts(ap, mnts, me);
		if (ret == -1 || ret == 0)
			return ret;

		if (me->ioctlfd != -1) {
			error(ap->logopt, "active direct mount %s", me->key);
			return -1;
		}
	}

	status = pthread_once(&key_mnt_params_once, key_mnt_params_init);
	if (status)
		fatal(status);

	mp = pthread_getspecific(key_mnt_direct_params);
	if (!mp) {
		mp = (struct mnt_params *) malloc(sizeof(struct mnt_params));
		if (!mp) {
			crit(ap->logopt,
			  "mnt_params value create failed for direct mount %s",
			  ap->path);
			return 0;
		}
		mp->options = NULL;

		status = pthread_setspecific(key_mnt_direct_params, mp);
		if (status) {
			free(mp);
			fatal(status);
		}
	}

	if (!mp->options) {
		mp->options = make_options_string(ap->path, ap->kpipefd, str_direct);
		if (!mp->options)
			return 0;
	}

	/* In case the directory doesn't exist, try to mkdir it */
	if (mkdir_path(me->key, 0555) < 0) {
		if (errno != EEXIST && errno != EROFS) {
			crit(ap->logopt,
			     "failed to create mount directory %s", me->key);
			return -1;
		}
		/* If we recieve an error, and it's EEXIST or EROFS we know
		   the directory was not created. */
		me->flags &= ~MOUNT_FLAG_DIR_CREATED;
	} else {
		/* No errors so the directory was successfully created */
		me->flags |= MOUNT_FLAG_DIR_CREATED;
	}

	map_name = me->mc->map->argv[0];

	ret = mount(map_name, me->key, "autofs", MS_MGC_VAL, mp->options);
	if (ret) {
		crit(ap->logopt, "failed to mount autofs path %s", me->key);
		goto out_err;
	}

	ret = stat(me->key, &st);
	if (ret == -1) {
		error(ap->logopt,
		      "failed to stat direct mount trigger %s", me->key);
		goto out_umount;
	}

	ops->open(ap->logopt, &ioctlfd, st.st_dev, me->key);
	if (ioctlfd < 0) {
		crit(ap->logopt, "failed to create ioctl fd for %s", me->key);
		goto out_umount;
	}

	ops->timeout(ap->logopt, ioctlfd, timeout);
	notify_mount_result(ap, me->key, timeout, str_direct);
	cache_set_ino_index(me->mc, me->key, st.st_dev, st.st_ino);
	ops->close(ap->logopt, ioctlfd);

	debug(ap->logopt, "mounted trigger %s", me->key);

	return 0;

out_umount:
	/* TODO: maybe force umount (-l) */
	umount(me->key);
out_err:
	if (me->flags & MOUNT_FLAG_DIR_CREATED)
		rmdir(me->key);

	return -1;
}