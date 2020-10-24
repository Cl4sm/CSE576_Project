static int
check_state(int fd, struct directio_context *ct, int sync, int timeout_secs)
{
	struct timespec	timeout = { .tv_nsec = 5 };
	struct io_event event;
	struct stat	sb;
	int		rc = PATH_UNCHECKED;
	long		r;

	if (fstat(fd, &sb) == 0) {
		LOG(4, "called for %x", (unsigned) sb.st_rdev);
	}
	if (sync > 0) {
		LOG(4, "called in synchronous mode");
		timeout.tv_sec  = timeout_secs;
		timeout.tv_nsec = 0;
	}

	if (!ct->running) {
		struct iocb *ios[1] = { &ct->io };

		LOG(3, "starting new request");
		memset(&ct->io, 0, sizeof(struct iocb));
		io_prep_pread(&ct->io, fd, ct->ptr, ct->blksize, 0);
		if (io_submit(ct->ioctx, 1, ios) != 1) {
			LOG(3, "io_submit error %i", errno);
			return PATH_UNCHECKED;
		}
	}
	ct->running++;

	errno = 0;
	r = io_getevents(ct->ioctx, 1L, 1L, &event, &timeout);

	if (r < 0 ) {
		LOG(3, "async io getevents returned %li (errno=%s)", r,
		    strerror(errno));
		ct->running = 0;
		rc = PATH_UNCHECKED;
	} else if (r < 1L) {
		if (ct->running > timeout_secs || sync) {
			struct iocb *ios[1] = { &ct->io };

			LOG(3, "abort check on timeout");
			r = io_cancel(ct->ioctx, ios[0], &event);
			/*
			 * Only reset ct->running if we really
			 * could abort the pending I/O
			 */
			if (r)
				LOG(3, "io_cancel error %i", errno);
			else
				ct->running = 0;
			rc = PATH_DOWN;
		} else {
			LOG(3, "async io pending");
			rc = PATH_PENDING;
		}
	} else {
		LOG(3, "io finished %lu/%lu", event.res, event.res2);
		ct->running = 0;
		rc = (event.res == ct->blksize) ? PATH_UP : PATH_DOWN;
	}

	return rc;
}
