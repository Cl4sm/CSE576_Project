static int afuse_opt_proc(void *data, const char *arg, int key,
			  struct fuse_args *outargs)
{
	/* Unused */
	(void)arg;
	(void)data;

	switch (key) {
	case KEY_HELP:
		usage(outargs->argv[0]);
		fuse_opt_add_arg(outargs, "-ho");
		fuse_main(outargs->argc, outargs->argv, &afuse_oper);
		exit(1);

	case KEY_FLUSHWRITES:
		user_options.flush_writes = true;
		return 0;

	case KEY_EXACT_GETATTR:
		user_options.exact_getattr = true;
		return 0;

	default:
		return 1;
	}
}