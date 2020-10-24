setup_thread_attr(pthread_attr_t *attr, size_t stacksize, int detached)
{
	if (pthread_attr_init(attr)) {
		fprintf(stderr, "can't initialize thread attr: %s\n",
			strerror(errno));
		exit(1);
	}
	if (stacksize < PTHREAD_STACK_MIN)
		stacksize = PTHREAD_STACK_MIN;

	if (pthread_attr_setstacksize(attr, stacksize)) {
		fprintf(stderr, "can't set thread stack size to %lu: %s\n",
			(unsigned long)stacksize, strerror(errno));
		exit(1);
	}
	if (detached && pthread_attr_setdetachstate(attr,
						    PTHREAD_CREATE_DETACHED)) {
		fprintf(stderr, "can't set thread to detached: %s\n",
			strerror(errno));
		exit(1);
	}
}
