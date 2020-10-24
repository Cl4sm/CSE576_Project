int
get_sys_max_fds(int *max_fds)
{
	FILE *file;
	int nr_open;
	int ret = 1;

	file = fopen("/proc/sys/fs/nr_open", "r");
	if (!file) {
		fprintf(stderr, "Cannot open /proc/sys/fs/nr_open : %s\n",
			strerror(errno));
		return 1;
	}
	if (fscanf(file, "%d", &nr_open) != 1) {
		fprintf(stderr, "Cannot read max open fds from /proc/sys/fs/nr_open");
		if (ferror(file))
			fprintf(stderr, " : %s\n", strerror(errno));
		else
			fprintf(stderr, "\n");
	} else {
		*max_fds = nr_open;
		ret = 0;
	}
	fclose(file);
	return ret;
}
