int do_expire(struct autofs_point *ap, const char *name, int namelen)
{
	char buf[PATH_MAX];
	int len, ret;

	if (*name != '/') {
		len = ncat_path(buf, sizeof(buf), ap->path, name, namelen);
	} else {
		len = snprintf(buf, PATH_MAX, "%s", name);
		if (len >= PATH_MAX)
			len = 0;
	}

	if (!len) {
		crit(ap->logopt, "path to long for buffer");
		return 1;
	}

	info(ap->logopt, "expiring path %s", buf);

	pthread_cleanup_push(master_source_lock_cleanup, ap->entry);
	master_source_readlock(ap->entry);
	ret = umount_multi(ap, buf, 1);
	if (ret == 0)
		info(ap->logopt, "expired %s", buf);
	else
		warn(ap->logopt, "couldn't complete expire of %s", buf);
	pthread_cleanup_pop(1);

	return ret;
}