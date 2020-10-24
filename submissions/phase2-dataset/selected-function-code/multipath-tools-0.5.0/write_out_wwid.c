static int
write_out_wwid(int fd, char *wwid) {
	int ret;
	off_t offset;
	char buf[WWID_SIZE + 3];

	ret = snprintf(buf, WWID_SIZE + 3, "/%s/\n", wwid);
	if (ret >= (WWID_SIZE + 3) || ret < 0){
		condlog(0, "can't format wwid for writing (%d) : %s",
			ret, strerror(errno));
		return -1;
	}
	offset = lseek(fd, 0, SEEK_END);
	if (offset < 0) {
		condlog(0, "can't seek to the end of wwids file : %s",
			strerror(errno));
		return -1;
	}
	if (write_all(fd, buf, strlen(buf)) != strlen(buf)) {
		condlog(0, "cannot write wwid to wwids file : %s",
			strerror(errno));
		if (ftruncate(fd, offset))
			condlog(0, "cannot truncate failed wwid write : %s",
				strerror(errno));
		return -1;
	}
	return 1;
}
