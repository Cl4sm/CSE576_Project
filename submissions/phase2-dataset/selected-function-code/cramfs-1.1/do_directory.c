{
	int pathlen = strlen(path);
	int count = i->size;
	unsigned long offset = i->offset << 2;
	char *newpath = malloc(pathlen + 256);

	if (!newpath) {
		die(FSCK_ERROR, 1, "malloc failed");
	}
	if (offset == 0 && count != 0) {
		die(FSCK_UNCORRECTED, 0, "directory inode has zero offset and non-zero size: %s", path);
	}
	if (offset != 0 && offset < start_dir) {
		start_dir = offset;
	}
	/* TODO: Do we need to check end_dir for empty case? */
	memcpy(newpath, path, pathlen);
	newpath[pathlen] = '/';
	pathlen++;
	if (opt_verbose) {
		print_node('d', i, path);
	}
	if (opt_extract) {
		if (mkdir(path, i->mode) < 0) {
			die(FSCK_ERROR, 1, "mkdir failed: %s", path);
		}
		change_file_status(path, i);
	}
	while (count > 0) {
		struct cramfs_inode *child = iget(offset);
		int size;
		int newlen = child->namelen << 2;

		size = sizeof(struct cramfs_inode) + newlen;
		count -= size;

		offset += sizeof(struct cramfs_inode);

		memcpy(newpath + pathlen, romfs_read(offset), newlen);
		newpath[pathlen + newlen] = 0;
		if (newlen == 0) {
			die(FSCK_UNCORRECTED, 0, "filename length is zero");
		}
		if ((pathlen + newlen) - strlen(newpath) > 3) {
			die(FSCK_UNCORRECTED, 0, "bad filename length");
		}
		expand_fs(newpath, child);

		offset += newlen;

		if (offset <= start_dir) {
			die(FSCK_UNCORRECTED, 0, "bad inode offset");
		}
		if (offset > end_dir) {
			end_dir = offset;
		}
		iput(child); /* free(child) */
	}
	free(newpath);
}
