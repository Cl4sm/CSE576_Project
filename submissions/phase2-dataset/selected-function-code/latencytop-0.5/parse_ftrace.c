{
	FILE *file;
	char line[PATH_MAX];
	file = fopen("/sys/kernel/debug/tracing/trace", "r");
	if (!file)
		return;
	while (!feof(file)) {
		char *c, *c2;
		memset(line, 0, PATH_MAX);
		fgets(line, PATH_MAX-1, file);
		c = strchr(line, '\n');
		if (c) *c = 0;
		c = strstr(line, "probe_do_fsync: Process ");
		if (!c)
			continue;
		c += 24;
		c2 = strchr(c, ' ');
		if (!c2)
			continue;
		*c2 = 0;
		c2++;
		c2 = strstr(c2, "fsync on ");
		if (!c2)
			continue;
		c2 += 9;
		report_file(c, c2);
	}
	fclose(file);
	sort_the_lot();
}
