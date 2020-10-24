open_file(char *file, int *can_write, char *header)
{
	int fd;
	struct stat s;

	if (ensure_directories_exist(file, 0700))
		return -1;
	*can_write = 1;
	fd = open(file, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		if (errno == EROFS) {
			*can_write = 0;
			condlog(3, "Cannot open file [%s] read/write. "
				" trying readonly", file);
			fd = open(file, O_RDONLY);
			if (fd < 0) {
				condlog(0, "Cannot open file [%s] "
					"readonly : %s", file, strerror(errno));
				return -1;
			}
		}
		else {
			condlog(0, "Cannot open file [%s] : %s", file,
				strerror(errno));
			return -1;
		}
	}
	if (*can_write && lock_file(fd, file) < 0)
		goto fail;

	memset(&s, 0, sizeof(s));
	if (fstat(fd, &s) < 0){
		condlog(0, "Cannot stat file %s : %s", file, strerror(errno));
		goto fail;
	}
	if (s.st_size == 0) {
		if (*can_write == 0)
			goto fail;
		/* If file is empty, write the header */
		size_t len = strlen(header);
		if (write_all(fd, header, len) != len) {
			condlog(0,
				"Cannot write header to file %s : %s", file,
				strerror(errno));
			/* cleanup partially written header */
			if (ftruncate(fd, 0))
				condlog(0, "Cannot truncate header : %s",
					strerror(errno));
			goto fail;
		}
		fsync(fd);
		condlog(3, "Initialized new file [%s]", file);
	}

	return fd;

fail:
	close(fd);
	return -1;
}
