void osm_log_v2(IN osm_log_t * p_log, IN osm_log_level_t verbosity,
		IN const int file_id, IN const char *p_str, ...)
{
	char buffer[LOG_ENTRY_SIZE_MAX];
	va_list args;
	int ret;
#ifdef __WIN__
	SYSTEMTIME st;
	uint32_t pid = GetCurrentThreadId();
#else
	struct timeval tv;
	pid_t pid = 0;
	time_t tim;
	struct tm result;
	uint64_t time_usecs;
	uint32_t usecs;
#endif				/* __WIN__ */

	/* If this is a call to syslog - always print it */
	if (!(verbosity & p_log->level)) {
		if (!(verbosity & p_log->per_mod_log_tbl[file_id]))
			return;
	}

	va_start(args, p_str);
#ifndef __WIN__
	if (p_log->log_prefix == NULL)
		vsprintf(buffer, p_str, args);
	else {
		int n = snprintf(buffer, sizeof(buffer), "%s: ", p_log->log_prefix);
		vsprintf(buffer + n, p_str, args);
	}
#else
	if (p_log->log_prefix == NULL)
		_vsnprintf(buffer, 1024, (LPSTR)p_str, args);
	else {
		int n = snprintf(buffer, sizeof(buffer), "%s: ", p_log->log_prefix);
		_vsnprintf(buffer + n, (1024 - n), (LPSTR)p_str, args);
	}
#endif
	va_end(args);

	/* this is a call to the syslog */
	if (verbosity & OSM_LOG_SYS) {
		syslog(LOG_INFO, "%s\n", buffer);

		/* SYSLOG should go to stdout too */
		if (p_log->out_port != stdout) {
			printf("%s\n", buffer);
			fflush(stdout);
		}
#ifdef __WIN__
		OsmReportState(buffer);
#endif				/* __WIN__ */
	}

	/* regular log to default out_port */
	cl_spinlock_acquire(&p_log->lock);

	if (p_log->max_size && p_log->count > p_log->max_size) {
		/* truncate here */
		fprintf(stderr,
			"osm_log: log file exceeds the limit %lu. Truncating.\n",
			p_log->max_size);
		truncate_log_file(p_log);
	}
#ifdef __WIN__
	GetLocalTime(&st);
_retry:
	ret =
	    fprintf(p_log->out_port,
		    "[%02d:%02d:%02d:%03d][%04X] 0x%02x -> %s",
		    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		    pid, verbosity, buffer);
#else
	gettimeofday(&tv, NULL);
	/* Convert the time of day into a microsecond timestamp */
	time_usecs = ((uint64_t) tv.tv_sec * 1000000) + (uint64_t) tv.tv_usec;
	tim = time_usecs / 1000000;
	usecs = time_usecs % 1000000;
	localtime_r(&tim, &result);
	pid = pthread_self();
_retry:
	ret =
	    fprintf(p_log->out_port,
		    "%s %02d %02d:%02d:%02d %06d [%04X] 0x%02x -> %s",
		    (result.tm_mon <
		     12 ? month_str[result.tm_mon] : "???"),
		    result.tm_mday, result.tm_hour, result.tm_min,
		    result.tm_sec, usecs, pid, verbosity, buffer);
#endif

	/*  flush log */
	if (ret > 0 &&
	    (p_log->flush || (verbosity & (OSM_LOG_ERROR | OSM_LOG_SYS)))
	    && fflush(p_log->out_port) < 0)
		ret = -1;

	if (ret >= 0) {
		log_exit_count = 0;
		p_log->count += ret;
	} else if (log_exit_count < 3) {
		log_exit_count++;
		if (errno == ENOSPC && p_log->max_size) {
			fprintf(stderr,
				"osm_log: write failed: %s. Truncating log file.\n",
				strerror(errno));
			truncate_log_file(p_log);
			goto _retry;
		}
		fprintf(stderr, "osm_log: write failed: %s\n", strerror(errno));
	}

	cl_spinlock_release(&p_log->lock);
}