int mount_autofs_offset(struct autofs_point *ap, struct mapent *me, const char *root, const char *offset)
{
	const char *str_offset = mount_type_str(t_offset);
	struct ioctl_ops *ops = get_ioctl_ops();
	char buf[MAX_ERR_BUF];
	struct mnt_params *mp;
	time_t timeout = me->source->exp_timeout;
	struct stat st;
	int ioctlfd, status, ret;
	const char *hosts_map_name = "-hosts";
	const char *map_name = hosts_map_name;
	const char *type;
	char mountpoint[PATH_MAX];

	if (ops->version && ap->flags & MOUNT_FLAG_REMOUNT) {
		ret = try_remount(ap, me, t_offset);
		if (ret == 1)
			return MOUNT_OFFSET_OK;
		/* Offset mount not found, fall thru and try to mount it */
		if (!(ret == -1 && errno == ENOENT))
			return MOUNT_OFFSET_FAIL;
	} else {
/*
		if (is_mounted(_PROC_MOUNTS, me->key, MNTS_AUTOFS)) {
			if (ap->state != ST_READMAP)
				warn(ap->logopt,
				     "trigger %s already mounted", me->key);
			return MOUNT_OFFSET_OK;
		}
*/
		if (me->ioctlfd != -1) {
			error(ap->logopt, "active offset mount %s", me->key);
			return MOUNT_OFFSET_FAIL;
		}
	}

	status = pthread_once(&key_mnt_params_once, key_mnt_params_init);
	if (status)
		fatal(status);

	mp = pthread_getspecific(key_mnt_offset_params);
	if (!mp) {
		mp = (struct mnt_params *) malloc(sizeof(struct mnt_params));
		if (!mp) {
			crit(ap->logopt,
			  "mnt_params value create failed for offset mount %s",
			  me->key);
			return MOUNT_OFFSET_OK;
		}
		mp->options = NULL;

		status = pthread_setspecific(key_mnt_offset_params, mp);
		if (status) {
			free(mp);
			fatal(status);
		}
	}

	if (!mp->options) {
		mp->options = make_options_string(ap->path, ap->kpipefd, str_offset);
		if (!mp->options)
			return MOUNT_OFFSET_OK;
	}

	strcpy(mountpoint, root);
	strcat(mountpoint, offset);

	/* In case the directory doesn't exist, try to mkdir it */
	if (mkdir_path(mountpoint, 0555) < 0) {
		if (errno == EEXIST) {
			/*
			 * If the mount point directory is a real mount
			 * and it isn't the root offset then it must be
			 * a mount that has been automatically mounted by
			 * the kernel NFS client.
			 */
			if (me->multi != me &&
			    is_mounted(_PROC_MOUNTS, mountpoint, MNTS_REAL))
				return MOUNT_OFFSET_IGNORE;

			/* 
			 * If we recieve an error, and it's EEXIST
			 * we know the directory was not created.
			 */
			me->flags &= ~MOUNT_FLAG_DIR_CREATED;
		} else if (errno == EACCES) {
			/*
			 * We require the mount point directory to exist when
			 * installing multi-mount triggers into a host
			 * filesystem.
			 *
			 * If it doesn't exist it is not a valid part of the
			 * mount heirachy.
			 */
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			debug(ap->logopt,
			     "can't create mount directory: %s, %s",
			     mountpoint, estr);
			return MOUNT_OFFSET_FAIL;
		} else {
			char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
			crit(ap->logopt,
			     "failed to create mount directory: %s, %s",
			     mountpoint, estr);
			return MOUNT_OFFSET_FAIL;
		}
	} else {
		/* No errors so the directory was successfully created */
		me->flags |= MOUNT_FLAG_DIR_CREATED;
	}

	debug(ap->logopt,
	      "calling mount -t autofs " SLOPPY " -o %s automount %s",
	      mp->options, mountpoint);

	type = ap->entry->maps->type;
	if (!type || strcmp(ap->entry->maps->type, "hosts"))
		map_name = me->mc->map->argv[0];

	ret = mount(map_name, mountpoint, "autofs", MS_MGC_VAL, mp->options);
	if (ret) {
		crit(ap->logopt,
		     "failed to mount offset trigger %s at %s",
		     me->key, mountpoint);
		goto out_err;
	}

	ret = stat(mountpoint, &st);
	if (ret == -1) {
		error(ap->logopt,
		     "failed to stat direct mount trigger %s", mountpoint);
		goto out_umount;
	}

	ops->open(ap->logopt, &ioctlfd, st.st_dev, mountpoint);
	if (ioctlfd < 0) {
		crit(ap->logopt, "failed to create ioctl fd for %s", mountpoint);
		goto out_umount;
	}

	ops->timeout(ap->logopt, ioctlfd, timeout);
	cache_set_ino_index(me->mc, me->key, st.st_dev, st.st_ino);
	if (ap->logopt & LOGOPT_DEBUG)
		notify_mount_result(ap, mountpoint, timeout, str_offset);
	else
		notify_mount_result(ap, me->key, timeout, str_offset);
	ops->close(ap->logopt, ioctlfd);

	debug(ap->logopt, "mounted trigger %s at %s", me->key, mountpoint);

	return MOUNT_OFFSET_OK;

out_umount:
	umount(mountpoint);
out_err:
	if (stat(mountpoint, &st) == 0 && me->flags & MOUNT_FLAG_DIR_CREATED)
		 rmdir_path(ap, mountpoint, st.st_dev);

	return MOUNT_OFFSET_FAIL;
}