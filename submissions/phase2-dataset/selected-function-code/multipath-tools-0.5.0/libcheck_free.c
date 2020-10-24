void libcheck_free (struct checker * c)
{
	struct directio_context * ct = (struct directio_context *)c->context;
	long flags;

	if (!ct)
		return;

	if (ct->reset_flags) {
		if ((flags = fcntl(c->fd, F_GETFL)) >= 0) {
			flags &= ~O_DIRECT;
			/* No point in checking for errors */
			fcntl(c->fd, F_SETFL, flags);
		}
	}

	if (ct->buf)
		free(ct->buf);
	io_destroy(ct->ioctx);
	free(ct);
}
