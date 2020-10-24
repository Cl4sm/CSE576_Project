int main(int argc, char *argv[])
{
	int res, opt, status;
	int logpri = -1;
	unsigned ghost, logging, daemon_check;
	unsigned dumpmaps, foreground, have_global_options;
	time_t timeout;
	time_t age = time(NULL);
	struct rlimit rlim;
	const char *options = "+hp:t:vmdD:fVrO:l:n:CF";
	static const struct option long_options[] = {
		{"help", 0, 0, 'h'},
		{"pid-file", 1, 0, 'p'},
		{"timeout", 1, 0, 't'},
		{"verbose", 0, 0, 'v'},
		{"debug", 0, 0, 'd'},
		{"define", 1, 0, 'D'},
		{"foreground", 0, 0, 'f'},
		{"random-multimount-selection", 0, 0, 'r'},
		{"negative-timeout", 1, 0, 'n'},
		{"dumpmaps", 0, 0, 'm'},
		{"global-options", 1, 0, 'O'},
		{"version", 0, 0, 'V'},
		{"set-log-priority", 1, 0, 'l'},
		{"dont-check-daemon", 0, 0, 'C'},
		{"force", 0, 0, 'F'},
		{0, 0, 0, 0}
	};

	sigfillset(&block_sigs);
	/* allow for the dropping of core files */
	sigdelset(&block_sigs, SIGABRT);
	sigdelset(&block_sigs, SIGBUS);
	sigdelset(&block_sigs, SIGSEGV);
	sigdelset(&block_sigs, SIGILL);
	sigdelset(&block_sigs, SIGFPE);
	sigdelset(&block_sigs, SIGTRAP);
	sigprocmask(SIG_BLOCK, &block_sigs, NULL);

	program = argv[0];

	defaults_read_config(0);

	kpkt_len = get_kpkt_len();
	timeout = defaults_get_timeout();
	ghost = defaults_get_browse_mode();
	logging = defaults_get_logging();
	global_selection_options = 0;
	global_options = NULL;
	have_global_options = 0;
	foreground = 0;
	dumpmaps = 0;
	daemon_check = 1;

	remove_empty_args(argv, &argc);

	opterr = 0;
	while ((opt = getopt_long(argc, argv, options, long_options, NULL)) != EOF) {
		switch (opt) {
		case 'h':
			usage();
			exit(0);

		case 'p':
			pid_file = optarg;
			break;

		case 't':
			timeout = getnumopt(optarg, opt);
			break;

		case 'v':
			logging |= LOGOPT_VERBOSE;
			break;

		case 'd':
			logging |= LOGOPT_DEBUG;
			break;

		case 'D':
			macro_parse_globalvar(optarg);
			break;

		case 'f':
			foreground = 1;
			break;

		case 'V':
			show_build_info();
			exit(0);

		case 'r':
			global_selection_options |= MOUNT_FLAG_RANDOM_SELECT;
			break;

		case 'n':
			global_negative_timeout = getnumopt(optarg, opt);
			break;

		case 'm':
			dumpmaps = 1;
			break;

		case 'O':
			if (!have_global_options) {
				global_options = strdup(optarg);
				have_global_options = 1;
				break;
			}
			printf("%s: global options already specified.\n",
				program);
			break;

		case 'l':
			if (isalpha(*optarg)) {
				logpri = convert_log_priority(optarg);
				if (logpri < 0) {
					fprintf(stderr, "Invalid log priority:"
						" %s\n", optarg);
					exit(1);
				}
			} else if (isdigit(*optarg)) {
				logpri = getnumopt(optarg, opt);
			} else {
				fprintf(stderr, "non-alphanumeric character "
					"found in log priority.  Aborting.\n");
				exit(1);
			}
			break;

		case 'C':
			daemon_check = 0;
			break;

		case 'F':
			do_force_unlink = 1;
			break;

		case '?':
		case ':':
			printf("%s: Ambiguous or unknown options\n", program);
			exit(1);
		}
	}

	if (logging & LOGOPT_VERBOSE)
		set_log_verbose();

	if (logging & LOGOPT_DEBUG)
		set_log_debug();

	if (geteuid() != 0) {
		fprintf(stderr, "%s: this program must be run by root.\n",
			program);
		exit(1);
	}

	/* Remove the options */
	argv += optind;
	argc -= optind;

	if (logpri >= 0) {
		int exit_code = 0;
		int i;

		/*
		 * The remaining argv elements are the paths for which
		 * log priorities must be changed.
		 */
		for (i = 0; i < argc; i++) {
			if (set_log_priority(argv[i], logpri) < 0)
				exit_code = 1;
		}
		if (argc < 1) {
			fprintf(stderr,
				"--set-log-priority requires a path.\n");
			exit_code = 1;
		}
		exit(exit_code);
	}

#if 0
	if (!load_autofs4_module()) {
		fprintf(stderr, "%s: can't load %s filesystem module.\n",
			program, FS_MODULE_NAME);
		exit(1);
	}
#endif

	/* Don't need the kernel module just to look at the configured maps */
	if (!dumpmaps && (!query_kproto_ver() || get_kver_major() < 5)) {
		fprintf(stderr,
			"%s: test mount forbidden or "
			"incorrect kernel protocol version, "
			"kernel protocol version 5.00 or above required.\n",
			program);
		exit(1);
	}

	res = getrlimit(RLIMIT_NOFILE, &rlim);
	if (res == -1 || rlim.rlim_max <= MAX_OPEN_FILES)  {
		rlim.rlim_cur = MAX_OPEN_FILES;
		rlim.rlim_max = MAX_OPEN_FILES;
	}
	res = setrlimit(RLIMIT_NOFILE, &rlim);
	if (res)
		printf("%s: can't increase open file limit - continuing",
			program);

#if ENABLE_CORES
	rlim.rlim_cur = RLIM_INFINITY;
	rlim.rlim_max = RLIM_INFINITY;
	res = setrlimit(RLIMIT_CORE, &rlim);
	if (res)
		printf("%s: can't increase core file limit - continuing",
			program);
#endif

	if (dumpmaps) {
		struct master_mapent *entry;
		struct list_head *head, *p;
		struct mapent_cache *nc;
		const char *type = NULL;
		const char *name = NULL;
		const char *master = NULL;

		if (argc > 0) {
			if (argc >= 2) {
				type = argv[0];
				name = argv[1];
			}
			if (argc == 3)
				master = argv[2];
		}

		if (master)
			master_list = master_new(NULL, timeout, ghost);
		else
			master_list = master_new(master, timeout, ghost);
		if (!master_list) {
			printf("%s: can't create master map", program);
			exit(1);
		}

		log_to_stderr();

		master_init_scan();

		nc = cache_init_null_cache(master_list);
		if (!nc) {
			printf("%s: failed to init null map cache for %s",
				program, master_list->name);
			exit(1);
		}
		master_list->nc = nc;

		lookup_nss_read_master(master_list, 0);
		if (type) {
			const char *map = basename(name);
			if (!map)
				printf("%s: invalid map name %s\n",
					program, name);
			else
				dump_map(master_list, type, map);
		} else
			master_show_mounts(master_list);

		head = &master_list->mounts;
		p = head->next;
		while (p != head) {
			entry = list_entry(p, struct master_mapent, list);
			p = p->next;
			master_free_mapent_sources(entry, 1);
			master_free_mapent(entry);
		}
		master_kill(master_list);

		exit(0);
	}

	if (argc == 0)
		master_list = master_new(NULL, timeout, ghost);
	else
		master_list = master_new(argv[0], timeout, ghost);

	if (!master_list) {
		printf("%s: can't create master map %s", program, argv[0]);
		exit(1);
	}

	become_daemon(foreground, daemon_check);

	if (pthread_attr_init(&th_attr)) {
		logerr("%s: failed to init thread attribute struct!",
		     program);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

	if (pthread_attr_init(&th_attr_detached)) {
		logerr("%s: failed to init thread attribute struct!",
		     program);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

	if (pthread_attr_setdetachstate(
			&th_attr_detached, PTHREAD_CREATE_DETACHED)) {
		logerr("%s: failed to set detached thread attribute!",
		     program);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

#ifdef _POSIX_THREAD_ATTR_STACKSIZE
	if (pthread_attr_setstacksize(
			&th_attr_detached, PTHREAD_STACK_MIN*64)) {
		logerr("%s: failed to set stack size thread attribute!",
		       program);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}
#endif

	info(logging, "Starting automounter version %s, master map %s",
		version, master_list->name);
	info(logging, "using kernel protocol version %d.%02d",
		get_kver_major(), get_kver_minor());

	status = pthread_key_create(&key_thread_stdenv_vars,
				key_thread_stdenv_vars_destroy);
	if (status) {
		logerr("%s: failed to create thread data key for std env vars!",
		       program);
		master_kill(master_list);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

	init_ioctl_ctl();

	if (!alarm_start_handler()) {
		logerr("%s: failed to create alarm handler thread!", program);
		master_kill(master_list);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

	if (!st_start_handler()) {
		logerr("%s: failed to create FSM handler thread!", program);
		master_kill(master_list);
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(1);
	}

#if defined(WITH_LDAP) && defined(LIBXML2_WORKAROUND)
	void *dh_xml2 = dlopen("libxml2.so", RTLD_NOW);
	if (!dh_xml2)
		dh_xml2 = dlopen("libxml2.so.2", RTLD_NOW);
	if (dh_xml2)
		xmlInitParser();
#endif
#ifdef TIRPC_WORKAROUND
	void *dh_tirpc = dlopen("libtirpc.so", RTLD_NOW);
	if (!dh_tirpc)
		dh_tirpc = dlopen("libtirpc.so.1", RTLD_NOW);
#endif

	if (!master_read_master(master_list, age, 0)) {
		master_kill(master_list);
		*pst_stat = 3;
		res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
		close(start_pipefd[1]);
		release_flag_file();
		exit(3);
	}

	/*
	 * Mmm ... reset force unlink umount so we don't also do this
	 * in future when we receive a HUP signal.
	 */
	do_force_unlink = 0;

	st_stat = 0;
	res = write(start_pipefd[1], pst_stat, sizeof(*pst_stat));
	close(start_pipefd[1]);

	state_mach_thid = pthread_self();
	statemachine(NULL);

	master_kill(master_list);

	if (pid_file) {
		unlink(pid_file);
		pid_file = NULL;
	}
	closelog();
	release_flag_file();

#ifdef TIRPC_WORKAROUND
	if (dh_tirpc)
		dlclose(dh_tirpc);
#endif
#if defined(WITH_LDAP) && defined( LIBXML2_WORKAROUND)
	if (dh_xml2) {
		xmlCleanupParser();
		dlclose(dh_xml2);
	}
#endif
	close_ioctl_ctl();

	info(logging, "autofs stopped");

	exit(0);
}