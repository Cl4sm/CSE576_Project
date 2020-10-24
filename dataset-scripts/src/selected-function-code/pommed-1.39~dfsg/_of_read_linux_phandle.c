void _of_read_linux_phandle(const char *path, struct device_node *node)
{
	int fd;
	char buf[PATH_MAX];
	struct stat fstats;
	uint32_t size;
	
	snprintf(buf, sizeof(buf), "%s%s", path, "linux,phandle");

	if (stat(buf, &fstats) < 0)
		return;

	size = fstats.st_size;
	if(!size)
		return;

	if ((fd = open(buf, O_RDONLY)) < 0)
		return;

	node->linux_phandle.len = size;
	node->linux_phandle.data = calloc(1, size);

	if (read(fd, node->linux_phandle.data, size) != size)
		of_error("read()");

	close(fd);
}