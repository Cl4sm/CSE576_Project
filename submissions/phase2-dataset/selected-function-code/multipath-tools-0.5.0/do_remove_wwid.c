do_remove_wwid(int fd, char *str) {
	char buf[4097];
	char *ptr;
	off_t start = 0;
	int bytes;

	while (1) {
		if (lseek(fd, start, SEEK_SET) < 0) {
			condlog(0, "wwid file read lseek failed : %s",
				strerror(errno));
			return -1;
		}
		bytes = read(fd, buf, 4096);
		if (bytes < 0) {
			if (errno == EINTR || errno == EAGAIN)
				continue;
			condlog(0, "failed to read from wwids file : %s",
				strerror(errno));
			return -1;
		}
		if (!bytes) /* didn't find wwid to remove */
			return 1;
		buf[bytes] = '\0';
		ptr = strstr(buf, str);
		if (ptr != NULL) {
			condlog(3, "found '%s'", str);
			if (lseek(fd, start + (ptr - buf), SEEK_SET) < 0) {
				condlog(0, "write lseek failed : %s",
						strerror(errno));
				return -1;
			}
			while (1) {
				if (write(fd, "#", 1) < 0) {
					if (errno == EINTR || errno == EAGAIN)
						continue;
					condlog(0, "failed to write to wwids file : %s", strerror(errno));
					return -1;
				}
				return 0;
			}
		}
		ptr = strrchr(buf, '\n');
		if (ptr == NULL) { /* shouldn't happen, assume it is EOF */
			condlog(4, "couldn't find newline, assuming end of file");
			return 1;
		}
		start = start + (ptr - buf) + 1;
	}
}
