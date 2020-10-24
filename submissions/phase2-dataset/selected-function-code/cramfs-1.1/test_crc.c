{
	void *buf;
	u32 crc;

	if (!(super.flags & CRAMFS_FLAG_FSID_VERSION_2)) {
#ifdef INCLUDE_FS_TESTS
		return;
#else /* not INCLUDE_FS_TESTS */
		die(FSCK_USAGE, 0, "unable to test CRC: old cramfs format");
#endif /* not INCLUDE_FS_TESTS */
	}

	crc = crc32(0L, Z_NULL, 0);

	buf = mmap(NULL, super.size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		buf = mmap(NULL, super.size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (buf != MAP_FAILED) {
			lseek(fd, 0, SEEK_SET);
			read(fd, buf, super.size);
		}
	}
	if (buf != MAP_FAILED) {
		((struct cramfs_super *) (buf+start))->fsid.crc = crc32(0L, Z_NULL, 0);
		crc = crc32(crc, buf+start, super.size-start);
		munmap(buf, super.size);
	}
	else {
		int retval;
		size_t length = 0;

		buf = malloc(4096);
		if (!buf) {
			die(FSCK_ERROR, 1, "malloc failed");
		}
		lseek(fd, start, SEEK_SET);
		for (;;) {
			retval = read(fd, buf, 4096);
			if (retval < 0) {
				die(FSCK_ERROR, 1, "read failed: %s", filename);
			}
			else if (retval == 0) {
				break;
			}
			if (length == 0) {
				((struct cramfs_super *) buf)->fsid.crc = crc32(0L, Z_NULL, 0);
			}
			length += retval;
			if (length > (super.size-start)) {
				crc = crc32(crc, buf, retval - (length - (super.size-start)));
				break;
			}
			crc = crc32(crc, buf, retval);
		}
		free(buf);
	}

	if (crc != super.fsid.crc) {
		die(FSCK_UNCORRECTED, 0, "crc error");
	}
}
