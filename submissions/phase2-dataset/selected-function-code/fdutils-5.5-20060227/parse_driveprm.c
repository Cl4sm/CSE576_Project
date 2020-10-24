{
	int found;

	drive->drivenum = getdrivenum(fd, &drive->buf);
	if(drive->drivenum < 0)
		return -1;

	ioctl(fd, FDRESET, FD_RESET_IF_RAWCMD);
	if (ioctl(fd, FDGETDRVPRM, & drive->drvprm ) < 0 ){
		perror("get drive characteristics");
		exit(1);
	}

	zero_all(ids, &mask);
	driveprmin = fopen(DRIVEPRMFILE, "r");

	if(driveprmin) {
		/* if the file doesn't exist, infer all info from the cmos type
		 * stored in the floppy driver */
		found = 0;
		driveprmlex(drive->drivenum, ids, sizeof(ids)/sizeof(ids[0]), 
			    &mask, &found);
		if(!found)
			zero_all(ids, &mask);
		fclose(driveprmin);
	}
	compute_params(drive);
	return 0;
}
