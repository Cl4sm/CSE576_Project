static void handle_fifo_message(struct autofs_point *ap, int fd)
{
	int ret;
	char buffer[PIPE_BUF];
	char *end;
	long pri;
	char buf[MAX_ERR_BUF];

	memset(buffer, 0, sizeof(buffer));
	ret = read(fd, &buffer, sizeof(buffer));
	if (ret < 0) {
		char *estr = strerror_r(errno, buf, MAX_ERR_BUF);
		warn(ap->logopt, "read on fifo returned error: %s", estr);
		return;
	}

	if (ret != 2) {
		debug(ap->logopt, "expected 2 bytes, received %d.", ret);
		return;
	}

	errno = 0;
	pri = strtol(buffer, &end, 10);
	if ((pri == LONG_MIN || pri == LONG_MAX) && errno == ERANGE) {
		debug(ap->logopt, "strtol reported an %s.  Failed to set "
		      "log priority.", pri == LONG_MIN ? "underflow" : "overflow");
		return;
	}
	if ((pri == 0 && errno == EINVAL) || end == buffer) {
		debug(ap->logopt, "priority is expected to be an integer "
		      "in the range 0-7 inclusive.");
		return;
	}

	if (pri > LOG_DEBUG || pri < LOG_EMERG) {
		debug(ap->logopt, "invalid log priority (%ld) received "
		      "on fifo", pri);
		return;
	}

	/*
	 * OK, the message passed all of the sanity checks.  The
	 * automounter actually only supports three log priorities.
	 * Everything is logged at log level debug, deamon messages
	 * and everything except debug messages are logged with the
	 * verbose setting and only error and critical messages are
	 * logged when debugging isn't enabled.
	 */
	if (pri >= LOG_WARNING) {
		if (pri == LOG_DEBUG) {
			set_log_debug_ap(ap);
			info(ap->logopt, "Debug logging set for %s", ap->path);
		} else {
			set_log_verbose_ap(ap);
			info(ap->logopt, "Verbose logging set for %s", ap->path);
		}
	} else {
		if (ap->logopt & LOGOPT_ANY)
			info(ap->logopt, "Basic logging set for %s", ap->path);
		set_log_norm_ap(ap);
	}
}