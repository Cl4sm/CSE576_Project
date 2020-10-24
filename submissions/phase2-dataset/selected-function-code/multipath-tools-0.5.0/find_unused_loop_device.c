extern char *
find_unused_loop_device (void)
{
	/* Just creating a device, say in /tmp, is probably a bad idea -
	   people might have problems with backup or so.
	   So, we just try /dev/loop[0-7]. */

	char dev[20];
	char *loop_formats[] = { "/dev/loop%d", "/dev/loop/%d" };
	int i, j, fd, first = 0, somedev = 0, someloop = 0, loop_known = 0;
	struct stat statbuf;
	struct loop_info loopinfo;
	FILE *procdev;

	if (stat("/dev/loop-control", &statbuf) == 0 &&
	    S_ISCHR(statbuf.st_mode)) {
		fd = open("/dev/loop-control", O_RDWR);
		if (fd >= 0)
			first = ioctl(fd, LOOP_CTL_GET_FREE);
		close(fd);
		if (first < 0)
			first = 0;
	}
	for (j = 0; j < SIZE(loop_formats); j++) {

	    for(i = first; i < 256; i++) {
		sprintf(dev, loop_formats[j], i);

		if (stat (dev, &statbuf) == 0 && S_ISBLK(statbuf.st_mode)) {
			somedev++;
			fd = open (dev, O_RDONLY);

			if (fd >= 0) {

				if(ioctl (fd, LOOP_GET_STATUS, &loopinfo) == 0)
					someloop++;		/* in use */

				else if (errno == ENXIO) {
					close (fd);
					return xstrdup(dev);/* probably free */
				}

				close (fd);
			}
			
			/* continue trying as long as devices exist */
			continue;
		}
		break;
	    }
	}

	/* Nothing found. Why not? */
	if ((procdev = fopen(PROC_DEVICES, "r")) != NULL) {
		char line[100];

		while (fgets (line, sizeof(line), procdev))

			if (strstr (line, " loop\n")) {
				loop_known = 1;
				break;
			}

		fclose(procdev);

		if (!loop_known)
			loop_known = -1;
	}

	if (!somedev)
		fprintf(stderr, "mount: could not find any device /dev/loop#");

	else if (!someloop) {

	    if (loop_known == 1)
		fprintf(stderr,
		    "mount: Could not find any loop device.\n"
		    "       Maybe /dev/loop# has a wrong major number?");
	    
	    else if (loop_known == -1)
		fprintf(stderr,
		    "mount: Could not find any loop device, and, according to %s,\n"
		    "       this kernel does not know about the loop device.\n"
		    "       (If so, then recompile or `modprobe loop'.)",
		      PROC_DEVICES);

	    else
		fprintf(stderr,
		    "mount: Could not find any loop device. Maybe this kernel does not know\n"
		    "       about the loop device (then recompile or `modprobe loop'), or\n"
		    "       maybe /dev/loop# has the wrong major number?");

	} else
		fprintf(stderr, "mount: could not find any free loop device");
	
	return 0;
}
