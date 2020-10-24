_remove_map (struct multipath * mpp, struct vectors * vecs,
	    int stop_waiter, int purge_vec)
{
	int i;

	condlog(4, "%s: remove multipath map", mpp->alias);

	/*
	 * stop the DM event waiter thread
	 */
	if (stop_waiter)
		stop_waiter_thread(mpp, vecs);

	/*
	 * clear references to this map
	 */
	orphan_paths(vecs->pathvec, mpp);

	if (purge_vec &&
	    (i = find_slot(vecs->mpvec, (void *)mpp)) != -1)
		vector_del_slot(vecs->mpvec, i);

	/*
	 * final free
	 */
	free_multipath(mpp, KEEP_PATHS);
}
