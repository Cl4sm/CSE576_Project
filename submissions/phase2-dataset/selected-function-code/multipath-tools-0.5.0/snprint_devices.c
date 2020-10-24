extern int
snprint_devices (char * buff, int len, struct vectors *vecs)
{
	DIR *blkdir;
	struct dirent *blkdev;
	struct stat statbuf;
	char devpath[PATH_MAX];
	char *devptr;
	int threshold = MAX_LINE_LEN;
	int fwd = 0;
	int r;

	struct path * pp;

	if (!(blkdir = opendir("/sys/block")))
		return 1;

	if ((len - fwd - threshold) <= 0)
		return len;
	fwd += snprintf(buff + fwd, len - fwd, "available block devices:\n");

	strcpy(devpath,"/sys/block/");
	while ((blkdev = readdir(blkdir)) != NULL) {
		if ((strcmp(blkdev->d_name,".") == 0) ||
		    (strcmp(blkdev->d_name,"..") == 0))
			continue;

		devptr = devpath + 11;
		*devptr = '\0';
		strncat(devptr, blkdev->d_name, PATH_MAX-12);
		if (stat(devpath, &statbuf) < 0)
			continue;

		if (S_ISDIR(statbuf.st_mode) == 0)
			continue;

		if ((len - fwd - threshold)  <= 0)
			return len;

		fwd += snprintf(buff + fwd, len - fwd, "    %s", devptr);
		pp = find_path_by_dev(vecs->pathvec, devptr);
		if (!pp) {
			r = filter_devnode(conf->blist_devnode,
					   conf->elist_devnode, devptr);
			if (r > 0)
				fwd += snprintf(buff + fwd, len - fwd,
						" devnode blacklisted, unmonitored");
			else if (r < 0)
				fwd += snprintf(buff + fwd, len - fwd,
						" devnode whitelisted, unmonitored");
		} else
			fwd += snprintf(buff + fwd, len - fwd,
					" devnode whitelisted, monitored");
		fwd += snprintf(buff + fwd, len - fwd, "\n");
	}
	closedir(blkdir);

	if (fwd > len)
		return len;
	return fwd;
}
