int main(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	char *temp_dir_name = my_malloc(strlen(TMP_DIR_TEMPLATE));
	strcpy(temp_dir_name, TMP_DIR_TEMPLATE);

	if (fuse_opt_parse(&args, &user_options, afuse_opts, afuse_opt_proc) ==
	    -1)
		return 1;

	// !!FIXME!! force single-threading for now as data structures are not locked
	fuse_opt_add_arg(&args, "-s");

	// Adjust user specified timeout from seconds to microseconds as required
	if (user_options.auto_unmount_delay != UINT64_MAX)
		user_options.auto_unmount_delay *= 1000000;

	auto_unmount_ph_init(&auto_unmount_ph);

	/**
	 * Install the SIGALRM signal handler.
	 */
	{
		struct sigaction act;
		act.sa_handler = &handle_auto_unmount_timer;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		while (sigaction(SIGALRM, &act, NULL) == -1 && errno == EINTR)
			continue;
	}

	// Check for required parameters
	if (!user_options.mount_command_template
	    || !user_options.unmount_command_template) {
		fprintf(stderr, "(Un)Mount command templates missing.\n\n");
		usage(argv[0]);
		fuse_opt_add_arg(&args, "-ho");
		fuse_main(args.argc, args.argv, &afuse_oper);

		return 1;
	}

	if (user_options.filter_file)
		load_mount_filter_file(user_options.filter_file);

	if (!(mount_point_directory = mkdtemp(temp_dir_name))) {
		fprintf(stderr,
			"Failed to create temporary mount point dir.\n");
		return 1;
	}

	{
		struct stat buf;
		if (lstat(mount_point_directory, &buf) == -1) {
			fprintf(stderr,
				"Failed to stat temporary mount point dir.\n");
			return 1;
		}
		mount_point_dev = buf.st_dev;
	}

	umask(0);

	// !!FIXME!! death by signal doesn't unmount fs
	return fuse_main(args.argc, args.argv, &afuse_oper);
}