int
cli_restore_all_queueing(void *v, char **reply, int *len, void *data)
{
	struct vectors * vecs = (struct vectors *)data;
	struct multipath *mpp;
	int i;

	condlog(2, "restore queueing (operator)");
	vector_foreach_slot(vecs->mpvec, mpp, i) {
		if (mpp->no_path_retry != NO_PATH_RETRY_UNDEF &&
		    mpp->no_path_retry != NO_PATH_RETRY_FAIL) {
			dm_queue_if_no_path(mpp->alias, 1);
			if (mpp->nr_active > 0)
				mpp->retry_tick = 0;
			else
				mpp->retry_tick = mpp->no_path_retry * conf->checkint;
		}
	}
	return 0;
}
