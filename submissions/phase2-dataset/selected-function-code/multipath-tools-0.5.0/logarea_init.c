static int logarea_init (int size)
{
	logdbg(stderr,"enter logarea_init\n");
	la = (struct logarea *)MALLOC(sizeof(struct logarea));

	if (!la)
		return 1;

	if (size < MAX_MSG_SIZE)
		size = DEFAULT_AREA_SIZE;

	la->start = MALLOC(size);
	if (!la->start) {
		FREE(la);
		return 1;
	}
	memset(la->start, 0, size);

	la->empty = 1;
	la->end = la->start + size;
	la->head = la->start;
	la->tail = la->start;

	la->buff = MALLOC(MAX_MSG_SIZE + sizeof(struct logmsg));

	if (!la->buff) {
		FREE(la->start);
		FREE(la);
		return 1;
	}
	return 0;

}
