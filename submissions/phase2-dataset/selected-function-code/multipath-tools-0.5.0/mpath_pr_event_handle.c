{
	pthread_t thread;
	int rc;
	pthread_attr_t attr;
	struct multipath * mpp;

	mpp = pp->mpp;

	if (!mpp->reservation_key)
		return -1;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	rc = pthread_create(&thread, NULL , mpath_pr_event_handler_fn, pp);
	if (rc) {
		condlog(0, "%s: ERROR; return code from pthread_create() is %d", pp->dev, rc);
		return -1;
	}
	pthread_attr_destroy(&attr);
	rc = pthread_join(thread, NULL);
	return 0;
}
