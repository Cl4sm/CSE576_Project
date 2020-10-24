static void do_file(char *path, struct cramfs_inode *i)
{
	unsigned long offset = i->offset << 2;
	int fd = 0;

	if (offset == 0 && i->size != 0) {
		die(FSCK_UNCORRECTED, 0, "file inode has zero offset and non-zero size");
	}
	if (i->size == 0 && offset != 0) {
		die(FSCK_UNCORRECTED, 0, "file inode has zero size and non-zero offset");
	}
	if (offset != 0 && offset < start_data) {
		start_data = offset;
	}
	if (opt_verbose) {
		print_node('f', i, path);
	}
	if (opt_extract) {
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, i->mode);
		if (fd < 0) {
			die(FSCK_ERROR, 1, "open failed: %s", path);
		}
	}
	if (i->size) {
		do_uncompress(path, fd, offset, i->size);
	}
	if (opt_extract) {
		close(fd);
		change_file_status(path, i);
	}
}
