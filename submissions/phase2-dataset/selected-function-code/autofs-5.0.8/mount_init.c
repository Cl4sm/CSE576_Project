int mount_init(void **context)
{
	char tmp1[] = "/tmp/autoXXXXXX", *t1_dir;
	char tmp2[] = "/tmp/autoXXXXXX", *t2_dir;
	int err;
	struct stat st1, st2;

	t1_dir = mkdtemp(tmp1);
	t2_dir = mkdtemp(tmp2);
	if (t1_dir == NULL || t2_dir == NULL) {
		if (t1_dir)
			rmdir(t1_dir);
		if (t2_dir)
			rmdir(t2_dir);
		return 0;
	}

	if (lstat(t1_dir, &st1) == -1)
		goto out;

	err = spawn_mount(LOGOPT_NONE, "-n", "--bind", t1_dir, t2_dir, NULL);
	if (err == 0 &&
	    lstat(t2_dir, &st2) == 0 &&
	    st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino) {
		bind_works = 1;
	}

	if (spawn_umount(LOGOPT_NONE, "-n", t2_dir, NULL) != 0)
		debug(LOGOPT_ANY, MODPREFIX "umount failed for %s", t2_dir);

out:
	rmdir(t1_dir);
	rmdir(t2_dir);

	return 0;
}