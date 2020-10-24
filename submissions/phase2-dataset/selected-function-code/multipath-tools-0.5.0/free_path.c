void
free_path (struct path * pp)
{
	if (!pp)
		return;

	if (checker_selected(&pp->checker))
		checker_put(&pp->checker);

	if (prio_selected(&pp->prio))
		prio_put(&pp->prio);

	if (pp->fd >= 0)
		close(pp->fd);

	if (pp->udev) {
		udev_device_unref(pp->udev);
		pp->udev = NULL;
	}

	FREE(pp);
}
