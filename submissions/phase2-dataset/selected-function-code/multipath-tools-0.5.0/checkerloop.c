checkerloop (void *ap)
{
	struct vectors *vecs;
	struct path *pp;
	int count = 0;
	unsigned int i;

	mlockall(MCL_CURRENT | MCL_FUTURE);
	vecs = (struct vectors *)ap;
	condlog(2, "path checkers start up");

	/*
	 * init the path check interval
	 */
	vector_foreach_slot (vecs->pathvec, pp, i) {
		pp->checkint = conf->checkint;
	}

	while (1) {
		struct timeval diff_time, start_time, end_time;
		int num_paths = 0;

		if (gettimeofday(&start_time, NULL) != 0)
			start_time.tv_sec = 0;
		pthread_cleanup_push(cleanup_lock, &vecs->lock);
		lock(vecs->lock);
		pthread_testcancel();
		condlog(4, "tick");
#ifdef USE_SYSTEMD
		if (conf->watchdog)
			sd_notify(0, "WATCHDOG=1");
#endif
		if (vecs->pathvec) {
			vector_foreach_slot (vecs->pathvec, pp, i) {
				num_paths += check_path(vecs, pp);
			}
		}
		if (vecs->mpvec) {
			defered_failback_tick(vecs->mpvec);
			retry_count_tick(vecs->mpvec);
		}
		if (count)
			count--;
		else {
			condlog(4, "map garbage collection");
			mpvec_garbage_collector(vecs);
			count = MAPGCINT;
		}

		lock_cleanup_pop(vecs->lock);
		if (start_time.tv_sec &&
		    gettimeofday(&end_time, NULL) == 0 &&
		    num_paths) {
			timersub(&end_time, &start_time, &diff_time);
			condlog(3, "checked %d path%s in %lu.%06lu secs",
				num_paths, num_paths > 1 ? "s" : "",
				diff_time.tv_sec, diff_time.tv_usec);
		}
		sleep(1);
	}
	return NULL;
}
