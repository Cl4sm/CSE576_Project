static void update_auto_unmount(mount_list_t * mount)
{
	if (user_options.auto_unmount_delay == UINT64_MAX)
		return;

	/* Get the current time */
	struct timeval tv;
	mount_list_t *min_mount;
	int64_t cur_time, next_time;
	gettimeofday(&tv, NULL);
	cur_time = from_timeval(&tv);

	if (mount) {
		/* Always remove first */
		if (mount->auto_unmount_time != INT64_MAX)
			auto_unmount_ph_remove(&auto_unmount_ph, mount);

		if (!mount->fd_list && !mount->dir_list) {
			mount->auto_unmount_time =
			    cur_time + user_options.auto_unmount_delay;
			auto_unmount_ph_insert(&auto_unmount_ph, mount);
		} else {
			mount->auto_unmount_time = INT64_MAX;
		}
	}
	min_mount = auto_unmount_ph_min(&auto_unmount_ph);
	next_time = min_mount ? min_mount->auto_unmount_time : INT64_MAX;

	if (next_time != auto_unmount_next_timeout) {
		struct itimerval itv;
		auto_unmount_next_timeout = next_time;

		if (next_time != INT64_MAX) {
			if (next_time > cur_time)
				to_timeval(&itv.it_value, next_time - cur_time);
			else	/* Timer is set to expire immediately --- set it to 1 instead */
				to_timeval(&itv.it_value, 1);
		} else {
			/* Disable the timer */
			to_timeval(&itv.it_value, 0);
		}
		to_timeval(&itv.it_interval, 0);
		if (setitimer(ITIMER_REAL, &itv, NULL) != 0) {
			perror("Error setting timer");
		}
	}
}