void _of_read_type(const char *path, struct device_node *node)
{
	int fd;
	char buf[PATH_MAX];
	struct stat fstats;
	uint32_t size;
	
	snprintf(buf, sizeof(buf),"%s%s", path, "device_type");

	if (stat(buf, &fstats) < 0)
		return;

	size = fstats.st_size;

	if ((fd = open(buf, O_RDONLY)) < 0)
		return;

	node->type = calloc(1, size);
	if(!node->type)
		of_error("calloc()");

	if (read(fd, node->type, size) != size)
		of_error("read()");

	close(fd);
}