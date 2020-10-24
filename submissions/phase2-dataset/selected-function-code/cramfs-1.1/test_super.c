	struct stat st;

	/* find the physical size of the file or block device */
	if (stat(filename, &st) < 0) {
		die(FSCK_ERROR, 1, "stat failed: %s", filename);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		die(FSCK_ERROR, 1, "open failed: %s", filename);
	}
	if (S_ISBLK(st.st_mode)) {
		if (ioctl(fd, BLKGETSIZE, length) < 0) {
			die(FSCK_ERROR, 1, "ioctl failed: unable to determine device size: %s", filename);
		}
		*length = *length * 512;
	}
	else if (S_ISREG(st.st_mode)) {
		*length = st.st_size;
	}
	else {
		die(FSCK_ERROR, 0, "not a block device or file: %s", filename);
	}

	if (*length < sizeof(struct cramfs_super)) {
		die(FSCK_UNCORRECTED, 0, "file length too short");
	}

	/* find superblock */
	if (read(fd, &super, sizeof(super)) != sizeof(super)) {
		die(FSCK_ERROR, 1, "read failed: %s", filename);
	}
	if (super.magic == CRAMFS_MAGIC) {
		*start = 0;
	}
	else if (*length >= (PAD_SIZE + sizeof(super))) {
		lseek(fd, PAD_SIZE, SEEK_SET);
		if (read(fd, &super, sizeof(super)) != sizeof(super)) {
			die(FSCK_ERROR, 1, "read failed: %s", filename);
		}
		if (super.magic == CRAMFS_MAGIC) {
			*start = PAD_SIZE;
		}
	}

	/* superblock tests */
	if (super.magic != CRAMFS_MAGIC) {
		die(FSCK_UNCORRECTED, 0, "superblock magic not found");
	}
	if (super.flags & ~CRAMFS_SUPPORTED_FLAGS) {
		die(FSCK_ERROR, 0, "unsupported filesystem features");
	}
	if (super.size < PAGE_CACHE_SIZE) {
		die(FSCK_UNCORRECTED, 0, "superblock size (%d) too small", super.size);
	}
	if (super.flags & CRAMFS_FLAG_FSID_VERSION_2) {
		if (super.fsid.files == 0) {
			die(FSCK_UNCORRECTED, 0, "zero file count");
		}
		if (*length < super.size) {
			die(FSCK_UNCORRECTED, 0, "file length too short");
		}
		else if (*length > super.size) {
			fprintf(stderr, "warning: file extends past end of filesystem\n");
		}
	}
	else {
		fprintf(stderr, "warning: old cramfs format\n");
	}
}
