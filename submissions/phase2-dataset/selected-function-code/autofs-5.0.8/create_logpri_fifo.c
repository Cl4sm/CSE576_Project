static int create_logpri_fifo(struct autofs_point *ap)
{
	int ret = -1;
	int fd;
	char *fifo_name;
	char buf[MAX_ERR_BUF];

	fifo_name = automount_path_to_fifo(ap->logopt, ap->path);
	if (!fifo_name) {
		crit(ap->logopt, "Failed to allocate memory!");
		goto out_free; /* free(NULL) is okay */
	}

	ret = unlink(fifo_name);
	if (ret != 0 && errno != ENOENT) {
		crit(ap->logopt,
		     "Failed to unlink FIFO. Is the automount daemon "
		     "already running?");
		goto out_free;
	}

	ret = mkfifo(fifo_name, S_IRUSR|S_IWUSR);
	if (ret != 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		crit(ap->logopt,
		     "mkfifo for %s failed: %s", fifo_name, estr);
		goto out_free;
	}

	fd = open_fd(fifo_name, O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		crit(ap->logopt,
		     "Failed to open %s: %s", fifo_name, estr);
		unlink(fifo_name);
		ret = -1;
		goto out_free;
	}

	ap->logpri_fifo = fd;

out_free:
	free(fifo_name);
	return ret;
}