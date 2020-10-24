child (void * param)
{
	pthread_t check_thr, uevent_thr, uxlsnr_thr, uevq_thr;
	pthread_attr_t log_attr, misc_attr, uevent_attr;
	struct vectors * vecs;
	struct multipath * mpp;
	int i;
#ifdef USE_SYSTEMD
	unsigned long checkint;
#endif
	int rc, pid_rc;
	char *envp;

	mlockall(MCL_CURRENT | MCL_FUTURE);
	sem_init(&exit_sem, 0, 0);
	signal_init();

	udev = udev_new();

	setup_thread_attr(&misc_attr, 64 * 1024, 1);
	setup_thread_attr(&uevent_attr, 128 * 1024, 1);
	setup_thread_attr(&waiter_attr, 32 * 1024, 1);

	if (logsink == 1) {
		setup_thread_attr(&log_attr, 64 * 1024, 0);
		log_thread_start(&log_attr);
		pthread_attr_destroy(&log_attr);
	}

	running_state = DAEMON_START;

#ifdef USE_SYSTEMD
	sd_notify(0, "STATUS=startup");
#endif
	condlog(2, "--------start up--------");
	condlog(2, "read " DEFAULT_CONFIGFILE);

	if (load_config(DEFAULT_CONFIGFILE, udev))
		goto failed;

	dm_drv_version(conf->version, TGT_MPATH);
	if (init_checkers()) {
		condlog(0, "failed to initialize checkers");
		goto failed;
	}
	if (init_prio()) {
		condlog(0, "failed to initialize prioritizers");
		goto failed;
	}

	setlogmask(LOG_UPTO(conf->verbosity + 3));

	envp = getenv("LimitNOFILE");

	if (envp) {
		condlog(2,"Using systemd provided open fds limit of %s", envp);
	} else if (conf->max_fds) {
		struct rlimit fd_limit;

		if (getrlimit(RLIMIT_NOFILE, &fd_limit) < 0) {
			condlog(0, "can't get open fds limit: %s",
				strerror(errno));
			fd_limit.rlim_cur = 0;
			fd_limit.rlim_max = 0;
		}
		if (fd_limit.rlim_cur < conf->max_fds) {
			fd_limit.rlim_cur = conf->max_fds;
			if (fd_limit.rlim_max < conf->max_fds)
				fd_limit.rlim_max = conf->max_fds;
			if (setrlimit(RLIMIT_NOFILE, &fd_limit) < 0) {
				condlog(0, "can't set open fds limit to "
					"%lu/%lu : %s",
					fd_limit.rlim_cur, fd_limit.rlim_max,
					strerror(errno));
			} else {
				condlog(3, "set open fds limit to %lu/%lu",
					fd_limit.rlim_cur, fd_limit.rlim_max);
			}
		}

	}

	vecs = gvecs = init_vecs();
	if (!vecs)
		goto failed;

	setscheduler();
	set_oom_adj();

	conf->daemon = 1;
	udev_set_sync_support(0);
#ifdef USE_SYSTEMD
	envp = getenv("WATCHDOG_USEC");
	if (envp && sscanf(envp, "%lu", &checkint) == 1) {
		/* Value is in microseconds */
		conf->max_checkint = checkint / 1000000;
		/* Rescale checkint */
		if (conf->checkint > conf->max_checkint)
			conf->checkint = conf->max_checkint;
		else
			conf->checkint = conf->max_checkint / 4;
		condlog(3, "enabling watchdog, interval %d max %d",
			conf->checkint, conf->max_checkint);
		conf->watchdog = conf->checkint;
	}
#endif
	/*
	 * Start uevent listener early to catch events
	 */
	if ((rc = pthread_create(&uevent_thr, &uevent_attr, ueventloop, udev))) {
		condlog(0, "failed to create uevent thread: %d", rc);
		goto failed;
	}
	pthread_attr_destroy(&uevent_attr);
	if ((rc = pthread_create(&uxlsnr_thr, &misc_attr, uxlsnrloop, vecs))) {
		condlog(0, "failed to create cli listener: %d", rc);
		goto failed;
	}
	/*
	 * fetch and configure both paths and multipaths
	 */
#ifdef USE_SYSTEMD
	sd_notify(0, "STATUS=configure");
#endif
	running_state = DAEMON_CONFIGURE;

	lock(vecs->lock);
	if (configure(vecs, 1)) {
		unlock(vecs->lock);
		condlog(0, "failure during configuration");
		goto failed;
	}
	unlock(vecs->lock);

	/*
	 * start threads
	 */
	if ((rc = pthread_create(&check_thr, &misc_attr, checkerloop, vecs))) {
		condlog(0,"failed to create checker loop thread: %d", rc);
		goto failed;
	}
	if ((rc = pthread_create(&uevq_thr, &misc_attr, uevqloop, vecs))) {
		condlog(0, "failed to create uevent dispatcher: %d", rc);
		goto failed;
	}
	pthread_attr_destroy(&misc_attr);

	/* Startup complete, create logfile */
	pid_rc = pidfile_create(DEFAULT_PIDFILE, daemon_pid);
	/* Ignore errors, we can live without */

	running_state = DAEMON_RUNNING;
#ifdef USE_SYSTEMD
	sd_notify(0, "READY=1\nSTATUS=running");
#endif

	/*
	 * exit path
	 */
	while(sem_wait(&exit_sem) != 0); /* Do nothing */

#ifdef USE_SYSTEMD
	sd_notify(0, "STATUS=shutdown");
#endif
	running_state = DAEMON_SHUTDOWN;
	lock(vecs->lock);
	if (conf->queue_without_daemon == QUE_NO_DAEMON_OFF)
		vector_foreach_slot(vecs->mpvec, mpp, i)
			dm_queue_if_no_path(mpp->alias, 0);
	remove_maps_and_stop_waiters(vecs);
	unlock(vecs->lock);

	pthread_cancel(check_thr);
	pthread_cancel(uevent_thr);
	pthread_cancel(uxlsnr_thr);
	pthread_cancel(uevq_thr);

	lock(vecs->lock);
	free_pathvec(vecs->pathvec, FREE_PATHS);
	vecs->pathvec = NULL;
	unlock(vecs->lock);
	/* Now all the waitevent threads will start rushing in. */
	while (vecs->lock.depth > 0) {
		sleep (1); /* This is weak. */
		condlog(3, "Have %d wait event checkers threads to de-alloc,"
			" waiting...", vecs->lock.depth);
	}
	pthread_mutex_destroy(vecs->lock.mutex);
	FREE(vecs->lock.mutex);
	vecs->lock.depth = 0;
	vecs->lock.mutex = NULL;
	FREE(vecs);
	vecs = NULL;

	cleanup_checkers();
	cleanup_prio();

	dm_lib_release();
	dm_lib_exit();

	/* We're done here */
	if (!pid_rc) {
		condlog(3, "unlink pidfile");
		unlink(DEFAULT_PIDFILE);
	}

	condlog(2, "--------shut down-------");

	if (logsink == 1)
		log_thread_stop();

	/*
	 * Freeing config must be done after condlog() and dm_lib_exit(),
	 * because logging functions like dlog() and dm_write_log()
	 * reference the config.
	 */
	free_config(conf);
	conf = NULL;
	udev_unref(udev);
	udev = NULL;
#ifdef _DEBUG_
	dbg_free_final(NULL);
#endif

#ifdef USE_SYSTEMD
	sd_notify(0, "ERRNO=0");
#endif
	exit(0);

failed:
#ifdef USE_SYSTEMD
	sd_notify(0, "ERRNO=1");
#endif
	exit(1);
}
