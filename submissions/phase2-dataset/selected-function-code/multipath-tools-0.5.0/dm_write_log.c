dm_write_log (int level, const char *file, int line, const char *f, ...)
{
	va_list ap;
	int thres;

	if (level > 6)
		level = 6;

	thres = (conf) ? conf->verbosity : 0;
	if (thres <= 3 || level > thres)
		return;

	va_start(ap, f);
	if (logsink < 1) {
		if (logsink == 0) {
			time_t t = time(NULL);
			struct tm *tb = localtime(&t);
			char buff[16];

			strftime(buff, sizeof(buff), "%b %d %H:%M:%S", tb);
			buff[sizeof(buff)-1] = '\0';

			fprintf(stdout, "%s | ", buff);
		}
		fprintf(stdout, "libdevmapper: %s(%i): ", file, line);
		vfprintf(stdout, f, ap);
		fprintf(stdout, "\n");
	} else {
		condlog(level, "libdevmapper: %s(%i): ", file, line);
		log_safe(level + 3, f, ap);
	}
	va_end(ap);

	return;
}
