static void *do_expire_indirect(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	struct pending_args *args, mt;
	struct autofs_point *ap;
	int status, state;

	args = (struct pending_args *) arg;

	pending_mutex_lock(args);

	memcpy(&mt, args, sizeof(struct pending_args));

	ap = mt.ap;

	args->signaled = 1;
	status = pthread_cond_signal(&args->cond);
	if (status)
		fatal(status);

	pending_mutex_unlock(args);

	pthread_cleanup_push(expire_send_fail, &mt);

	status = do_expire(mt.ap, mt.name, mt.len);
	pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &state);
	if (status)
		ops->send_fail(ap->logopt,
			       ap->ioctlfd, mt.wait_queue_token, -status);
	else
		ops->send_ready(ap->logopt,
				ap->ioctlfd, mt.wait_queue_token);
	pthread_setcancelstate(state, NULL);

	pthread_cleanup_pop(0);

	return NULL;
}