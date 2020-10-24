{
	unsigned long pgsize = getpagesize();
	struct directio_context * ct;
	long flags;

	ct = malloc(sizeof(struct directio_context));
	if (!ct)
		return 1;
	memset(ct, 0, sizeof(struct directio_context));

	if (io_setup(1, &ct->ioctx) != 0) {
		condlog(1, "io_setup failed");
		free(ct);
		return 1;
	}

	if (ioctl(c->fd, BLKBSZGET, &ct->blksize) < 0) {
		MSG(c, "cannot get blocksize, set default");
		ct->blksize = 512;
	}
	if (ct->blksize > 4096) {
		/*
		 * Sanity check for DASD; BSZGET is broken
		 */
		ct->blksize = 4096;
	}
	if (!ct->blksize)
		goto out;
	ct->buf = (unsigned char *)malloc(ct->blksize + pgsize);
	if (!ct->buf)
		goto out;

	flags = fcntl(c->fd, F_GETFL);
	if (flags < 0)
		goto out;
	if (!(flags & O_DIRECT)) {
		flags |= O_DIRECT;
		if (fcntl(c->fd, F_SETFL, flags) < 0)
			goto out;
		ct->reset_flags = 1;
	}

	ct->ptr = (unsigned char *) (((unsigned long)ct->buf + pgsize - 1) &
		  (~(pgsize - 1)));

	/* Sucessfully initialized, return the context. */
	c->context = (void *) ct;
	return 0;

out:
	if (ct->buf)
		free(ct->buf);
	io_destroy(ct->ioctx);
	free(ct);
	return 1;
}
