static int set_log_priority(const char *path, int priority)
{
	int fd;
	char *fifo_name;
	char buf[2];

	if (priority > LOG_DEBUG || priority < LOG_EMERG) {
		fprintf(stderr, "Log priority %d is invalid.\n", priority);
		fprintf(stderr, "Please spcify a number in the range 0-7.\n");
		return -1;
	}

	/*
	 * This is an ascii based protocol, so we want the string
	 * representation of the integer log priority.
	 */
	snprintf(buf, sizeof(buf), "%d", priority);

	fifo_name = automount_path_to_fifo(LOGOPT_NONE, path);
	if (!fifo_name) {
		fprintf(stderr, "%s: Failed to allocate memory!\n",
			__FUNCTION__);
		return -1;
	}

	/*
	 * Specify O_NONBLOCK so that the open will fail if there is no
	 * daemon reading from the other side of the FIFO.
	 */
	fd = open(fifo_name, O_WRONLY|O_NONBLOCK);
	if (fd < 0) {
		fprintf(stderr, "%s: open of %s failed with %s\n",
			__FUNCTION__, fifo_name, strerror(errno));
		fprintf(stderr, "%s: perhaps the fifo wasn't setup,"
			" please check your log for more information\n", __FUNCTION__);
		free(fifo_name);
		return -1;
	}

	if (write(fd, buf, sizeof(buf)) != sizeof(buf)) {
		fprintf(stderr, "Failed to change logging priority.  ");
		fprintf(stderr, "write to fifo failed: %s.\n",
			strerror(errno));
		close(fd);
		free(fifo_name);
		return -1;
	}
	close(fd);
	free(fifo_name);
	fprintf(stdout, "Successfully set log priority for %s.\n", path);

	return 0;
}