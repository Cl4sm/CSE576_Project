static void do_special_inode(char *path, struct cramfs_inode *i)
{
	dev_t devtype = 0;
	char type;

	if (i->offset) {	/* no need to shift offset */
		die(FSCK_UNCORRECTED, 0, "special file has non-zero offset: %s", path);
	}
	if (S_ISCHR(i->mode)) {
		devtype = i->size;
		type = 'c';
	}
	else if (S_ISBLK(i->mode)) {
		devtype = i->size;
		type = 'b';
	}
	else if (S_ISFIFO(i->mode)) {
		if (i->size != 0) {
			die(FSCK_UNCORRECTED, 0, "fifo has non-zero size: %s", path);
		}
		type = 'p';
	}
	else if (S_ISSOCK(i->mode)) {
		if (i->size != 0) {
			die(FSCK_UNCORRECTED, 0, "socket has non-zero size: %s", path);
		}
		type = 's';
	}
	else {
		die(FSCK_UNCORRECTED, 0, "bogus mode: %s (%o)", path, i->mode);
		return;		/* not reached */
	}

	if (opt_verbose) {
		print_node(type, i, path);
	}

	if (opt_extract) {
		if (mknod(path, i->mode, devtype) < 0) {
			die(FSCK_ERROR, 1, "mknod failed: %s", path);
		}
		change_file_status(path, i);
	}
}
