struct device_node *_of_get_phandle(const char *path, const uint32_t * phandle)
{
	struct node_property_t props;
	struct device_node *tmp = NULL;
	uint32_t size, val = 0;
	struct stat fstats;
	int fd;
	char *ptr;
	char *name;

	if (stat(path, &fstats) < 0)
		return NULL;

	size = fstats.st_size;
	if(!size)
		return NULL;

	props.data = malloc(size);
	if(!props.data)
		of_error("calloc()");

	props.len = size;

	if ((fd = open(path, O_RDONLY)) < 0)
		of_error("open()");

	if (read(fd, props.data, size) != size)
		of_error("read()");

	close(fd);

	val = _of_phandle_to_int(props);
	free(props.data);

	if (val == *phandle) {
		ptr = strdup(path);
		_of_remove_filename(ptr);
		name = _of_read_name(ptr, NULL);
		tmp = _of_populate_node(path, name);
		free(ptr);
		free(name);
	}

	return tmp;
}