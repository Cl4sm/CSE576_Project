{
	int ret;
	struct directio_context * ct = (struct directio_context *)c->context;

	if (!ct)
		return PATH_UNCHECKED;

	ret = check_state(c->fd, ct, c->sync, c->timeout);

	switch (ret)
	{
	case PATH_UNCHECKED:
		MSG(c, MSG_DIRECTIO_UNKNOWN);
		break;
	case PATH_DOWN:
		MSG(c, MSG_DIRECTIO_DOWN);
		break;
	case PATH_UP:
		MSG(c, MSG_DIRECTIO_UP);
		break;
	case PATH_PENDING:
		MSG(c, MSG_DIRECTIO_PENDING);
		break;
	default:
		break;
	}
	return ret;
}
