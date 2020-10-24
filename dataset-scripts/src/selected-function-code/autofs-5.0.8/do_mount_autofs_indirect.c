static int do_mount_autofs_indirect(struct autofs_point *ap, const char *root)
{
	const char *str_indirect = mount_type_str(t_indirect);
	struct ioctl_ops *ops = get_ioctl_ops();
	time_t timeout = ap->entry->maps->exp_timeout;
	char *options = NULL;
	const char *hosts_map_name = "-hosts";
	const char *map_name = hosts_map_name;
	const char *type;
	struct stat st;
	struct mnt_list *mnts;
	int ret;

	ap->exp_runfreq = (timeout + CHECK_RATIO - 1) / CHECK_RATIO;

	if (ops->version && !do_force_unlink) {
		ap->flags |= MOUNT_FLAG_REMOUNT;
		ret = try_remount(ap, NULL, t_indirect);
		ap->flags &= ~MOUNT_FLAG_REMOUNT;
		if (ret == 1)
			return 0;
		if (ret == 0)
			return -1;
	} else {
		mnts = get_mnt_list(_PROC_MOUNTS, ap->path, 1);
		if (mnts) {
			ret = unlink_mount_tree(ap, mnts);
			free_mnt_list(mnts);
			if (!ret) {
				error(ap->logopt,
				      "already mounted as other than autofs "
				      "or failed to unlink entry in tree");
				goto out_err;
			}
		}
	}

	options = make_options_string(ap->path, ap->kpipefd, str_indirect);
	if (!options) {
		error(ap->logopt, "options string error");
		goto out_err;
	}

	/* In case the directory doesn't exist, try to mkdir it */
	if (mkdir_path(root, 0555) < 0) {
		if (errno != EEXIST && errno != EROFS) {
			crit(ap->logopt,
			     "failed to create autofs directory %s",
			     root);
			goto out_err;
		}
		/* If we recieve an error, and it's EEXIST or EROFS we know
		   the directory was not created. */
		ap->flags &= ~MOUNT_FLAG_DIR_CREATED;
	} else {
		/* No errors so the directory was successfully created */
		ap->flags |= MOUNT_FLAG_DIR_CREATED;
	}

	type = ap->entry->maps->type;
	if (!type || strcmp(ap->entry->maps->type, "hosts"))
		map_name = ap->entry->maps->argv[0];

	ret = mount(map_name, root, "autofs", MS_MGC_VAL, options);
	if (ret) {
		crit(ap->logopt,
		     "failed to mount autofs path %s at %s", ap->path, root);
		goto out_rmdir;
	}

	free(options);
	options = NULL;

	ret = stat(root, &st);
	if (ret == -1) {
		crit(ap->logopt,
		     "failed to stat mount for autofs path %s", ap->path);
		goto out_umount;
	}

	if (ops->open(ap->logopt, &ap->ioctlfd, st.st_dev, root)) {
		crit(ap->logopt,
		     "failed to create ioctl fd for autofs path %s", ap->path);
		goto out_umount;
	}

	ap->dev = st.st_dev;	/* Device number for mount point checks */

	ops->timeout(ap->logopt, ap->ioctlfd, timeout);
	if (ap->logopt & LOGOPT_DEBUG)
		notify_mount_result(ap, root, timeout, str_indirect);
	else
		notify_mount_result(ap, ap->path, timeout, str_indirect);

	return 0;

out_umount:
	umount(root);
out_rmdir:
	if (ap->flags & MOUNT_FLAG_DIR_CREATED)
		rmdir(root);
out_err:
	if (options)
		free(options);
	close(ap->state_pipe[0]);
	close(ap->state_pipe[1]);
	close(ap->pipefd);
	close(ap->kpipefd);

	return -1;
}