int spawn_bind_mount(unsigned logopt, ...)
{
	va_list arg;
	int argc;
	char **argv, **p;
	char prog[] = PATH_MOUNT;
	char arg0[] = PATH_MOUNT;
	char bind[] = "--bind";
	char argn[] = "-n";
	/* In case we need to use the fake option to mount */
	char arg_fake[] = "-f";
	unsigned int options;
	unsigned int retries = MTAB_LOCK_RETRIES;
	int update_mtab = 1, ret, printed = 0;
	char buf[PATH_MAX + 1];

	/* If we use mount locking we can't validate the location */
#ifdef ENABLE_MOUNT_LOCKING
	options = SPAWN_OPT_LOCK;
#else
	options = SPAWN_OPT_ACCESS;
#endif

	/*
	 * Alloc 2 extra slots, one for the bind option and one in case
	 * we need to use the "-f" option
	 */
	va_start(arg, logopt);
	for (argc = 2; va_arg(arg, char *); argc++);
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

	if (!(argv = alloca(sizeof(char *) * (argc + 2))))
		return -1;

	argv[0] = arg0;
	argv[1] = bind;

	va_start(arg, logopt);
	if (update_mtab)
		p = argv + 2;
	else {
		argv[2] = argn;
		p = argv + 3;
	}
	while ((*p++ = va_arg(arg, char *)));
	va_end(arg);

	while (retries--) {
		ret = do_spawn(logopt, -1, options, prog, (const char **) argv);
		if (ret & MTAB_NOTUPDATED) {
			struct timespec tm = {3, 0};

			/*
			 * If the mount succeeded but the mtab was not
			 * updated, then retry the mount with the -f (fake)
			 * option to just update the mtab.
			 */
			if (!printed) {
				debug(logopt, "mount failed with error code 16"
				      ", retrying with the -f option");
				printed = 1;
			}

			/*
			 * Move the last two args so do_spawn() can find the
			 * mount target.
			 */
			if (!argv[argc]) {
				argv[argc + 1] = NULL;
				argv[argc] = argv[argc - 1];
				argv[argc - 1] = argv[argc - 2];
				argv[argc - 2] = arg_fake;
			}

			nanosleep(&tm, NULL);

			continue;
		}
		break;
	}

	/* This is not a fatal error */
	if (ret == MTAB_NOTUPDATED) {
		/*
		 * Version 5 requires that /etc/mtab be in sync with
		 * /proc/mounts. If we're unable to update matb after
		 * retrying then we have no choice but umount the mount
		 * and return a fail.
		 */
		warn(logopt,
		     "Unable to update the mtab file, forcing mount fail!");
		umount(argv[argc]);
		ret = MNT_FORCE_FAIL;
	}

	return ret;
}