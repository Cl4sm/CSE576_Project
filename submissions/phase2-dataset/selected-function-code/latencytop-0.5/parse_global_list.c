void parse_global_list(void)
{
	FILE *file;
	char *ln;
	size_t dummy;
	file = fopen("/proc/latency_stats","r+");
	if (!file) {
		fprintf(stderr, "Please enable the CONFIG_LATENCYTOP configuration in your kernel.\n");
		fprintf(stderr, "Exiting...\n");
		exit(EXIT_FAILURE);
	}
	/* wipe first line */
	ln = NULL;
	if (getline(&ln, &dummy, file) < 0) {
		free(ln);
		fclose(file);
		return;
	}
	free(ln);
	total_time = 0;
	total_count = 0;
	while (!feof(file)) {
		struct latency_line *line;
		char *c;
		ln = NULL;
		if (getline(&ln, &dummy, file) < 0) {
			free(ln);
			break;
		}
		if (strlen(ln)<2) {
			free(ln);
			break;
		}
		line = malloc(sizeof(struct latency_line));
		memset(line, 0, sizeof(struct latency_line));
		line->count = strtoull(ln, &c, 10);
		line->time = strtoull(c, &c, 10);
		line->max = strtoull(c, &c, 10);
		total_time += line->time;
		total_count += line->count;
		fixup_reason(line, c);
		add_to_global(line);
		free(ln);
		ln = NULL;
	}
	/* reset for next time */
	fprintf(file, "erase\n");
	fclose(file);
}
