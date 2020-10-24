check_wwids_file(char *wwid, int write_wwid)
{
	int fd, can_write, found, ret;
	FILE *f;
	fd = open_file(conf->wwids_file, &can_write, WWIDS_FILE_HEADER);
	if (fd < 0)
		return -1;

	f = fdopen(fd, "r");
	if (!f) {
		condlog(0,"can't fdopen wwids file : %s", strerror(errno));
		close(fd);
		return -1;
	}
	found = lookup_wwid(f, wwid);
	if (found) {
		ret = 0;
		goto out;
	}
	if (!write_wwid) {
		ret = -1;
		goto out;
	}
	if (!can_write) {
		condlog(0, "wwids file is read-only. Can't write wwid");
		ret = -1;
		goto out;
	}

	if (fflush(f) != 0) {
		condlog(0, "cannot fflush wwids file stream : %s",
			strerror(errno));
		ret = -1;
		goto out;
	}

	ret = write_out_wwid(fd, wwid);
out:
	fclose(f);
	return ret;
}
