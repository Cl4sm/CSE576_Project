	unsigned int rq_type, struct prout_param_descriptor * paramp, int noisy)
{
	struct prout_param param;
	param.rq_servact = rq_servact;
	param.rq_scope  = rq_scope;
	param.rq_type   = rq_type;
	param.paramp    = paramp;
	param.noisy = noisy;
	param.status = -1;

	pthread_t thread;
	pthread_attr_t attr;
	int rc;

	memset(&thread, 0, sizeof(thread));
	strncpy(param.dev, dev, FILE_NAME_SIZE);
	/* Initialize and set thread joinable attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	rc = pthread_create(&thread, &attr, mpath_prout_pthread_fn, (void *)(&param));
	if (rc){
		condlog (3, "%s: failed to create thread %d", dev, rc);
		exit(-1);
	}
	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	rc = pthread_join(thread, NULL);

	return (param.status);
}
