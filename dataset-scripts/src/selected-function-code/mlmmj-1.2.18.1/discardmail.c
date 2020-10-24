int discardmail(const char *old, const char *new, time_t age)
{
	struct stat st;
	time_t t;
	int fd, ret = 0;

	fd = open(old, O_RDWR);
	if(fd < 0)
		return 0;

	if(myexcllock(fd) < 0) {
		close(fd);
		return 0;
	}

	stat(old, &st);
	t = time(NULL);

	if(t - st.st_mtime > age) {
		if(rename(old, new) < 0)
			ret = 0;
		else
			ret = 1;
	}

	close(fd);
	return ret;
}