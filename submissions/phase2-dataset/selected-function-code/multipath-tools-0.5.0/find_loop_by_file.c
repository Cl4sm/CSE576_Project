find_loop_by_file (const char * filename)
{
	char dev[64];
	char *loop_formats[] = { "/dev/loop%d", "/dev/loop/%d" };
	int i, j, fd;
	struct stat statbuf;
	struct loop_info loopinfo;

	for (j = 0; j < SIZE(loop_formats); j++) {

		for (i = 0; i < 256; i++) {
			sprintf (dev, loop_formats[j], i);

			if (stat (dev, &statbuf) != 0 ||
			    !S_ISBLK(statbuf.st_mode))
				continue;

			fd = open (dev, O_RDONLY);

			if (fd < 0)
				break;

			if (ioctl (fd, LOOP_GET_STATUS, &loopinfo) != 0) {
				close (fd);
				continue;
			}

			if (0 == strcmp(filename, loopinfo.lo_name)) {
				close (fd);
				return xstrdup(dev); /*found */
			}

			close (fd);
			continue;
		}
	}
	return NULL;
}
