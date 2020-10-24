struct device_node *_of_get_name(const char *path, const char *name)
{
	int fd;
	uint32_t size;
	char buf[PATH_MAX];
	struct device_node *tmp = NULL;
	struct stat fstats;

	if (stat(path, &fstats) < 0)
		return NULL;
	
	size = fstats.st_size;
	if(!size)
		return NULL;

	if ((fd = open(path, O_RDONLY)) < 0)
		of_error("open()");

	if (read(fd, buf, size) != size)
		of_error("read()");

	if (memcmp(buf, name, size))
		goto out;		

	tmp = _of_populate_node(path, name);

      out:

	close(fd);

	return tmp;
}