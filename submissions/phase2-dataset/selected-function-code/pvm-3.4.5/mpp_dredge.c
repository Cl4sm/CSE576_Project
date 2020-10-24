mpp_dredge()
{
	int i;
	struct task *tp;

	PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
	for (i = 0; i < maxpidtid; i++)
		if (pidtids[i].pt_stat == ST_FINISH) {
			if (tp = task_find(pidtids[i].pt_tid)) {
				PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
				task_cleanup(tp);
				task_free(tp);
				PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
			} else
				pidtids[i].pt_stat = ST_EXIT;
		}
	PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	return 0;
}
