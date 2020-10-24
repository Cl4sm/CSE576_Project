static int unlink_active_mounts(struct autofs_point *ap, struct mnt_list *mnts, struct mapent *me)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct list_head list;

	INIT_LIST_HEAD(&list);

	if (tree_get_mnt_list(mnts, &list, me->key, 1)) {
		if (ap->state == ST_READMAP) {
			time_t tout = me->source->exp_timeout;
			int save_ioctlfd, ioctlfd;

			save_ioctlfd = ioctlfd = me->ioctlfd;

			if (ioctlfd == -1)
				ops->open(ap->logopt,
					  &ioctlfd, me->dev, me->key);

			if (ioctlfd < 0) {
				error(ap->logopt,
				     "failed to create ioctl fd for %s",
				     me->key);
				return 0;
			}

			ops->timeout(ap->logopt, ioctlfd, tout);

			if (save_ioctlfd == -1)
				ops->close(ap->logopt, ioctlfd);

			return 0;
		}
	}

	if (!unlink_mount_tree(ap, &list)) {
		debug(ap->logopt,
		      "already mounted as other than autofs "
		      "or failed to unlink entry in tree");
		return 0;
	}

	return 1;
}