int destroy_logpri_fifo(struct autofs_point *ap)
{
	int ret = -1;
	int fd = ap->logpri_fifo;
	char *fifo_name;
	char buf[MAX_ERR_BUF];

	if (fd == -1)
		return 0;

	fifo_name = automount_path_to_fifo(ap->logopt, ap->path);
	if (!fifo_name) {
		crit(ap->logopt, "Failed to allocate memory!");
		goto out_free; /* free(NULL) is okay */
	}

	ap->logpri_fifo = -1;

	ret = close(fd);
	if (ret != 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		warn(ap->logopt,
		     "close for fifo %s: %s", fifo_name, estr);
	}

	ret = unlink(fifo_name);
	if (ret != 0) {
		warn(ap->logopt,
		     "Failed to unlink FIFO. Was the fifo created OK?");
	}

out_free:
	free(fifo_name);
	return ret;
}