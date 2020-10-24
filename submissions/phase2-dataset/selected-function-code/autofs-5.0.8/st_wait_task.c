int st_wait_task(struct autofs_point *ap, enum states state, unsigned int seconds)
{
	unsigned int wait = 0;
	unsigned int duration = 0;
	int ret = 0;

	while (1) {
		struct timespec t = { 0, 200000000 };
		struct timespec r;

		while (nanosleep(&t, &r) == -1 && errno == EINTR)
			memcpy(&t, &r, sizeof(struct timespec));

		if (wait++ == 4) {
			wait = 0;
			duration++;
		}

		if (!st_task_active(ap, state)) {
			ret = 1;
			break;
		}

		if (seconds && duration >= seconds)
			break;
	}

	return ret;
}