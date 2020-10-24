void write_progress(void *_vpninfo, int level, const char *fmt, ...)
{
	FILE *outf = level ? stdout : stderr;
	va_list args;

	if (cookieonly)
		outf = stderr;

	if (verbose >= level) {
		if (timestamp) {
			char ts[64];
			time_t t = time(NULL);
			struct tm *tm = localtime(&t);

			strftime(ts, 64, "[%Y-%m-%d %H:%M:%S] ", tm);
			fprintf(outf, "%s", ts);
		}
		va_start(args, fmt);
		vfprintf(outf, fmt, args);
		va_end(args);
		fflush(outf);
	}
}