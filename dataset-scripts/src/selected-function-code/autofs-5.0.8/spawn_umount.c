int spawn_umount(unsigned logopt, ...)
{
	va_list arg;
	int argc;
	char **argv, **p;
	char prog[] = PATH_UMOUNT;
	char arg0[] = PATH_UMOUNT;
	char argn[] = "-n";
	unsigned int options;
	unsigned int retries = MTAB_LOCK_RETRIES;
	int update_mtab = 1, ret, printed = 0;
	unsigned int wait = defaults_get_umount_wait();
	char buf[PATH_MAX + 1];

#ifdef ENABLE_MOUNT_LOCKING
	options = SPAWN_OPT_LOCK;
#else
	options = SPAWN_OPT_NONE;
#endif

	va_start(arg, logopt);
	for (argc = 1; va_arg(arg, char *); argc++);
	va_end(arg);

	ret = readlink(_PATH_MOUNTED, buf, PATH_MAX);
	if (ret != -1) {
		buf[ret] = '\0';
		if (!strcmp(buf, _PROC_MOUNTS) ||
		    !strcmp(buf, _PROC_SELF_MOUNTS)) {
			debug(logopt,
			      "mtab link detected, passing -n to mount");
			argc++;
			update_mtab = 0;
		}
	}

	if (!(argv = alloca(sizeof(char *) * argc + 1)))
		return -1;

	argv[0] = arg0;

	va_start(arg, logopt);
	if (update_mtab)
		p = argv + 1;
	else {
		argv[1] = argn;
		p = argv + 2;
	}
	while ((*p++ = va_arg(arg, char *)));
	va_end(arg);

	while (retries--) {
		ret = do_spawn(logopt, wait, options, prog, (const char **) argv);
		if (ret & MTAB_NOTUPDATED) {
			/*
			 * If the mount succeeded but the mtab was not
			 * updated, then retry the umount just to update
			 * the mtab.
			 */
			if (!printed) {
				debug(logopt, "mount failed with error code 16"
				      ", retrying with the -f option");
				printed = 1;
			}
		} else {
			/*
			 * umount does not support the "fake" option.  Thus,
			 * if we got a return value of MTAB_NOTUPDATED the
			 * first time, that means the umount actually
			 * succeeded.  Then, a following umount will fail
			 * due to the fact that nothing was mounted on the
			 * mount point. So, report this as success.
			 */
			if (retries < MTAB_LOCK_RETRIES - 1)
				ret = 0;
			break;
		}
	}

	/* This is not a fatal error */
	if (ret == MTAB_NOTUPDATED) {
		warn(logopt, "Unable to update the mtab file, /proc/mounts "
		     "and /etc/mtab will differ");
		ret = 0;
	}

	return ret;
}