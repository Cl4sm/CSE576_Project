{
	int fd;
	char *buf;

	fd = open(file, O_RDONLY);
	if (fd < 0) {
		die(MKFS_ERROR, 1, "open failed: %s", file);
	}
	buf = mmap(NULL, image_length, PROT_READ, MAP_PRIVATE, fd, 0);
	if (buf == MAP_FAILED) {
		die(MKFS_ERROR, 1, "mmap failed");
	}
	memcpy(base + offset, buf, image_length);
	munmap(buf, image_length);
	close (fd);
	/* Pad up the image_length to a 4-byte boundary */
	while (image_length & 3) {
		*(base + offset + image_length) = '\0';
		image_length++;
	}
	return (offset + image_length);
}
