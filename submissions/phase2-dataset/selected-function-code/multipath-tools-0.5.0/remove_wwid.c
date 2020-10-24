remove_wwid(char *wwid) {
	int fd, len, can_write;
	char *str;
	int ret = -1;

	len = strlen(wwid) + 4; /* two slashes the newline and a zero byte */
	str = malloc(len);
	if (str == NULL) {
		condlog(0, "can't allocate memory to remove wwid : %s",
			strerror(errno));
		return -1;
	}
	if (snprintf(str, len, "/%s/\n", wwid) >= len) {
		condlog(0, "string overflow trying to remove wwid");
		goto out;
	}
	condlog(3, "removing line '%s' from wwids file", str);
	fd = open_file(conf->wwids_file, &can_write, WWIDS_FILE_HEADER);
	if (fd < 0)
		goto out;
	if (!can_write) {
		condlog(0, "cannot remove wwid. wwids file is read-only");
		goto out_file;
	}
	ret = do_remove_wwid(fd, str);

out_file:
	close(fd);
out:
	free(str);
	return ret;
}
