struct device_node *_of_get_type(const char *path, const char *type)
{
	int fd;
	char buf[PATH_MAX];
	char *name;
	char *ptr;
	struct device_node *tmp = NULL;
	struct stat fstats;
	uint32_t size;

	if (stat(path, &fstats) < 0) {
		exit(EXIT_FAILURE);
	}

	size = fstats.st_size;
	if(!size)
		return NULL;
	
	if ((fd = open(path, O_RDONLY)) < 0)
		of_error("open()");

	if (read(fd, buf, size) != size)
		of_error("read()");

	if (memcmp(buf, type, size))
		goto out;

	ptr = strdup(path);

	_of_remove_filename(ptr);

	name = _of_read_name(ptr, NULL);

	tmp = _of_populate_node(path, name);

	free(ptr);
	free(name);
	
      out:

	close(fd);

	return tmp;
}