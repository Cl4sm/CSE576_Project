{
	if (read_all(fd, len, sizeof(*len)) != sizeof(*len)) {
		(*buf) = NULL;
		*len = 0;
		return -1;
	}
	if (len == 0) {
		(*buf) = NULL;
		return 0;
	}
	(*buf) = MALLOC(*len);
	if (!*buf)
		return -1;
	if (read_all(fd, *buf, *len) != *len) {
		FREE(*buf);
		(*buf) = NULL;
		*len = 0;
		return -1;
	}
	return 0;
}
