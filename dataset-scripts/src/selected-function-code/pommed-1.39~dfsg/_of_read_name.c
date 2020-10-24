char *_of_read_name(const char *path, struct device_node *node)
{
	int fd;
	char *tmp;
	char buf[PATH_MAX];
	struct stat fstats;
	uint32_t size;
	
	snprintf(buf, sizeof(buf), "%s%s", path, "name");

	if (stat(buf, &fstats) < 0)
		return NULL;

	size = fstats.st_size;

	if ((fd = open(buf, O_RDONLY)) < 0)
		return NULL;

	if (node) {
		node->name = calloc(1, size);
		if(!node->name)
			of_error("calloc()");

		tmp = node->name;
	} else {
		tmp = calloc(1, size);
		if(!tmp)
			of_error("calloc()");
	}

	if (read(fd, tmp, size) != size)
		of_error("read()");

	close(fd);

	return tmp;
}