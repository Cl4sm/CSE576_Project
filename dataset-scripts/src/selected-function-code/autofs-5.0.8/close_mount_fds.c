void close_mount_fds(struct autofs_point *ap)
{
	/*
	 * Since submounts look after themselves the parent never knows
	 * it needs to close the ioctlfd for offset mounts so we have
	 * to do it here. If the cache entry isn't found then there aren't
	 * any offset mounts.
	 */
	if (ap->submount)
		lookup_source_close_ioctlfd(ap->parent, ap->path);

	close(ap->state_pipe[0]);
	close(ap->state_pipe[1]);
	ap->state_pipe[0] = -1;
	ap->state_pipe[1] = -1;

	if (ap->pipefd >= 0)
		close(ap->pipefd);

	if (ap->kpipefd >= 0)
		close(ap->kpipefd);

	return;
}