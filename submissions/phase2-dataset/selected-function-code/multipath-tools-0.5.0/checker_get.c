void checker_get (struct checker * dst, char * name)
{
	struct checker * src = checker_lookup(name);

	if (!dst)
		return;

	if (!src) {
		dst->check = NULL;
		return;
	}
	dst->fd = src->fd;
	dst->sync = src->sync;
	strncpy(dst->name, src->name, CHECKER_NAME_LEN);
	strncpy(dst->message, src->message, CHECKER_MSG_LEN);
	dst->check = src->check;
	dst->init = src->init;
	dst->free = src->free;
	dst->handle = NULL;
	src->refcount++;
}
