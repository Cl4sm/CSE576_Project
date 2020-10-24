static time_t trtime(char *s) {
	/* XXX: this function may not be particularly portable to
	 * other platforms, e.g. *BSDs, Windows */
	struct tm tm;
	time_t ret;

	if(sscanf(s, "%4u%2u%2u-%2u%2u%2u", &tm.tm_year, &tm.tm_mon,
				&tm.tm_mday, &tm.tm_hour, &tm.tm_min,
				&tm.tm_sec) != 6) {
		return (time_t)0;
	}
	tm.tm_year = tm.tm_year - 1900;
	tm.tm_mon --;
	tm.tm_wday = 0; /* ignored */
	tm.tm_yday = 0; /* ignored */
	tm.tm_isdst = -1; /* forces check for summer time */
	
	/*
	if (getenv("TZ") == NULL) {
		fprintf(stderr, "Failed to get the current TZ");
		return (time_t)0;
	}
	*/
	if (putenv("TZ=Pacific/Auckland")) {
		perror("putenv");
		return (time_t)0;
	}
	tzset();
	ret = mktime(&tm);

	return ret;
}