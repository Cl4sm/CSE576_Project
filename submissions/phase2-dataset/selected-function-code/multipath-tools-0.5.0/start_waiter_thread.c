int start_waiter_thread (struct multipath *mpp, struct vectors *vecs)
{
	struct event_thread *wp;

	if (!mpp)
		return 0;

	wp = alloc_waiter();

	if (!wp)
		goto out;

	strncpy(wp->mapname, mpp->alias, WWID_SIZE);
	wp->vecs = vecs;

	if (pthread_create(&wp->thread, &waiter_attr, waitevent, wp)) {
		condlog(0, "%s: cannot create event checker", wp->mapname);
		goto out1;
	}
	mpp->waiter = wp->thread;
	condlog(2, "%s: event checker started", wp->mapname);

	return 0;
out1:
	free_waiter(wp);
	mpp->waiter = (pthread_t)0;
out:
	condlog(0, "failed to start waiter thread");
	return 1;
}
