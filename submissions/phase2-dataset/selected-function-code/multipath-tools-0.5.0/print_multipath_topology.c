print_multipath_topology (struct multipath * mpp, int verbosity)
{
	int resize;
	char *buff = NULL;
	char *old = NULL;
	int len, maxlen = MAX_LINE_LEN * MAX_LINES;

	buff = MALLOC(maxlen);
	do {
		if (!buff) {
			if (old)
				FREE(old);
			condlog(0, "couldn't allocate memory for list: %s\n",
				strerror(errno));
			return;
		}

		len = snprint_multipath_topology(buff, maxlen, mpp, verbosity);
		resize = (len == maxlen - 1);

		if (resize) {
			maxlen *= 2;
			old = buff;
			buff = REALLOC(buff, maxlen);
		}
	} while (resize);
	printf("%s", buff);
}
