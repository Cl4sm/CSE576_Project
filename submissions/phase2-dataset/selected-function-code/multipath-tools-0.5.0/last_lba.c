static uint64_t
last_lba(int filedes)
{
	int rc;
	uint64_t sectors = 0;
	struct stat s;
	memset(&s, 0, sizeof (s));
	rc = fstat(filedes, &s);
	if (rc == -1) {
		fprintf(stderr, "last_lba() could not stat: %s\n",
			strerror(errno));
		return 0;
	}

	if (S_ISBLK(s.st_mode)) {
		sectors = _get_num_sectors(filedes);
	} else {
		fprintf(stderr,
			"last_lba(): I don't know how to handle files with mode %x\n",
			s.st_mode);
		sectors = 1;
	}

	return sectors ? sectors - 1 : 0;
}
