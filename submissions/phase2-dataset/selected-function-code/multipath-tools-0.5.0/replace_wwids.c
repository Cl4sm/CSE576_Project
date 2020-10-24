int
replace_wwids(vector mp)
{
	int i, fd, can_write;
	struct multipath * mpp;
	size_t len;
	int ret = -1;

	fd = open_file(conf->wwids_file, &can_write, WWIDS_FILE_HEADER);
	if (fd < 0)
		goto out;
	if (!can_write) {
		condlog(0, "cannot replace wwids. wwids file is read-only");
		goto out_file;
	}
	if (ftruncate(fd, 0) < 0) {
		condlog(0, "cannot truncate wwids file : %s", strerror(errno));
		goto out_file;
	}
	if (lseek(fd, 0, SEEK_SET) < 0) {
		condlog(0, "cannot seek to the start of the file : %s",
			strerror(errno));
		goto out_file;
	}
	len = strlen(WWIDS_FILE_HEADER);
	if (write_all(fd, WWIDS_FILE_HEADER, len) != len) {
		condlog(0, "Can't write wwid file header : %s",
			strerror(errno));
		/* cleanup partially written header */
		if (ftruncate(fd, 0) < 0)
			condlog(0, "Cannot truncate header : %s",
				strerror(errno));
		goto out_file;
	}
	if (!mp || !mp->allocated) {
		ret = 0;
		goto out_file;
	}
	vector_foreach_slot(mp, mpp, i) {
		if (write_out_wwid(fd, mpp->wwid) < 0)
			goto out_file;
	}
	ret = 0;
out_file:
	close(fd);
out:
	return ret;
}
