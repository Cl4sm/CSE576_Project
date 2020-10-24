void *of_find_property(struct device_node *node, const char *name, int *plen)
{
	char buf[PATH_MAX]={0};
	uint8_t *property;
	uint32_t size;
	struct stat fstats;
	int fd;
	

	strcat(strcat(buf, node->full_path), name);

	if (stat(buf, &fstats) < 0)
		return NULL;

	size = fstats.st_size;

	property = malloc(size);
	if(!property)
		of_error("malloc()");

	if ((fd = open(buf, O_RDONLY)) < 0)
		of_error("open()");

	if ((*plen = read(fd, property, size)) != size)
		of_error("read()");
		
	close(fd);

	return property;
}