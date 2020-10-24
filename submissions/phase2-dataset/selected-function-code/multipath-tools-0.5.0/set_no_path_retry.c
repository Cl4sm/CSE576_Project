set_no_path_retry(struct multipath *mpp)
{
	mpp->retry_tick = 0;
	mpp->nr_active = pathcount(mpp, PATH_UP) + pathcount(mpp, PATH_GHOST);
	select_no_path_retry(mpp);

	switch (mpp->no_path_retry) {
	case NO_PATH_RETRY_UNDEF:
		break;
	case NO_PATH_RETRY_FAIL:
		dm_queue_if_no_path(mpp->alias, 0);
		break;
	case NO_PATH_RETRY_QUEUE:
		dm_queue_if_no_path(mpp->alias, 1);
		break;
	default:
		dm_queue_if_no_path(mpp->alias, 1);
		if (mpp->nr_active == 0) {
			/* Enter retry mode */
			mpp->retry_tick = mpp->no_path_retry * conf->checkint;
			condlog(1, "%s: Entering recovery mode: max_retries=%d",
				mpp->alias, mpp->no_path_retry);
		}
		break;
	}
}
