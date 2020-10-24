void expire_cleanup(void *arg)
{
	struct ioctl_ops *ops = get_ioctl_ops();
	pthread_t thid = pthread_self();
	struct expire_args *ec;
	struct autofs_point *ap;
	int success;
	enum states next = ST_INVAL;

	ec = (struct expire_args *) arg;
	ap = ec->ap;
	success = ec->status;

	st_mutex_lock();

	debug(ap->logopt,
	      "got thid %lu path %s stat %d",
	      (unsigned long) thid, ap->path, success);

	/* Check to see if expire process finished */
	if (thid == ap->exp_thread) {
		unsigned int idle;
		int rv;

		ap->exp_thread = 0;

		switch (ap->state) {
		case ST_EXPIRE:
			/* FALLTHROUGH */
		case ST_PRUNE:
			/*
			 * If we're a submount and we've just pruned or
			 * expired everything away, try to shut down.
			 *
			 * Since we use the the fact that a mount will not
			 * expire for at least ap->exp_timeout to avoid a
			 * mount <-> expire race we need to wait before
			 * letting a submount expire away. We also need
			 * them to go away fairly quickly so the owner
			 * mount expires in a reasonable time. Just skip
			 * one expire check after it's no longer busy before
			 * allowing it to shutdown.
			 */
			if (ap->submount && !success) {
				rv = ops->askumount(ap->logopt, ap->ioctlfd, &idle);
				if (!rv && idle && ap->submount > 1) {
					next = ST_SHUTDOWN_PENDING;
					break;
				}

				if (ap->submount++ == 0)
					ap->submount = 2;
			}

			if (ap->state == ST_EXPIRE && !ap->submount)
				alarm_add(ap, ap->exp_runfreq);

			/* FALLTHROUGH */

		case ST_READY:
			next = ST_READY;
			break;

		case ST_SHUTDOWN_PENDING:
			/*
			 * If we reveive a mount request while trying to
			 * shutdown return to ready state unless we have
			 * been signaled to shutdown.
			 */
			rv = ops->askumount(ap->logopt, ap->ioctlfd, &idle);
			if (!rv && !idle && !ap->shutdown) {
				next = ST_READY;
				if (!ap->submount)
					alarm_add(ap, ap->exp_runfreq);
				break;
			}

			next = ST_SHUTDOWN;
#ifdef ENABLE_IGNORE_BUSY_MOUNTS
			break;
#else
			if (success == 0)
				break;

			/* Failed shutdown returns to ready */
			warn(ap->logopt, "filesystem %s still busy", ap->path);
			if (!ap->submount)
				alarm_add(ap, ap->exp_runfreq);
			next = ST_READY;
			break;
#endif

		case ST_SHUTDOWN_FORCE:
			next = ST_SHUTDOWN;
			break;

		default:
			error(ap->logopt, "bad state %d", ap->state);
		}

		if (next != ST_INVAL) {
			debug(ap->logopt,
			  "sigchld: exp %lu finished, switching from %d to %d",
			  (unsigned long) thid, ap->state, next);
		}
	}

	st_set_done(ap);

	if (next != ST_INVAL)
		__st_add_task(ap, next);

	st_mutex_unlock();

	return;
}