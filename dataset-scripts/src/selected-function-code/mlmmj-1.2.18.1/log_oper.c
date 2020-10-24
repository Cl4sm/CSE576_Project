int log_oper(const char *prefix, const char *basename, const char *fmt, ...)
{
	int fd, statres;
	char ct[26], *logstr, *logfilename, *tmp, log_msg[256];
	struct stat st;
	time_t t;
	va_list ap;
	size_t i;

	logfilename = concatstr(3, prefix, "/", basename);
	statres = lstat(logfilename, &st);
	if(statres < 0 && errno != ENOENT) {
		log_error(LOG_ARGS, "Could not stat logfile %s", logfilename);
		myfree(logfilename);
		return -1;
	}
	
	if(statres >= 0 && st.st_size > (off_t)OPLOGSIZE) {
		tmp = concatstr(2, logfilename, ".rotated");
		if(rename(logfilename, tmp) < 0) {
			log_error(LOG_ARGS, "Could not rename %s,%s",
					logfilename, tmp);
		}
		myfree(tmp);
	}
	
	fd = open(logfilename, O_RDWR|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);
	if(fd < 0) {
		log_error(LOG_ARGS, "Could not open %s", logfilename);
		myfree(logfilename);
		return -1;
	}

	if((time(&t) == (time_t)-1) || (ctime_r(&t, ct) == NULL))
		strncpy(ct, "Unknown time", sizeof(ct));
	else
		ct[24] = '\0';

	if(myexcllock(fd) < 0) {
		log_error(LOG_ARGS, "Could not lock %s", logfilename);
		myfree(logfilename);
		return -1;
	}

	va_start(ap, fmt);
	i = vsnprintf(log_msg, sizeof(log_msg), fmt, ap);
	if(i < 0) {
		va_end(ap);
		log_error(LOG_ARGS, "Failed to format log message: %s", fmt);
		return -1;
	}
	if(i > sizeof(log_msg))
		log_error(LOG_ARGS, "Log message truncated");

	va_end(ap);

	logstr = concatstr(4, ct, " ", log_msg, "\n");
	if(writen(fd, logstr, strlen(logstr)) < 0)
		log_error(LOG_ARGS, "Could not write to %s", logfilename);
	
	close(fd);
	myfree(logfilename);
	myfree(logstr);
	
	return 0;
}