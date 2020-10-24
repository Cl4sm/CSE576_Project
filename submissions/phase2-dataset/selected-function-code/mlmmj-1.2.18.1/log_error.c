void log_error(const char *file, int line, const char *errstr,
	const char *fmt, ...)
{
	static int syslog_is_open = 0;
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	if (!log_name) log_name = "mlmmj-UNKNOWN";

#ifdef HAVE_SYSLOG
	if(!syslog_is_open) {
		openlog(log_name, LOG_PID|LOG_CONS, LOG_MAIL);
		syslog_is_open = 1;
	}
	syslog(LOG_ERR, "%s:%d: %s: %s", file, line, buf, errstr);
#else
	fprintf(stderr, "%s[%d]: %s:%d: %s: %s\n", log_name, (int)getpid(),
			file, line, buf, errstr);
#endif
}