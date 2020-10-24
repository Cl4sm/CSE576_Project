{
	struct logmsg * src = (struct logmsg *)la->head;
	struct logmsg * dst = (struct logmsg *)buff;
	struct logmsg * lst = (struct logmsg *)la->tail;

	if (la->empty)
		return 1;

	int len = strlen((char *)&src->str) * sizeof(char) +
		  sizeof(struct logmsg) + 1;

	dst->prio = src->prio;
	memcpy(dst, src,  len);

	if (la->tail == la->head)
		la->empty = 1; /* we purge the last logmsg */
	else {
		la->head = src->next;
		lst->next = la->head;
	}
	logdbg(stderr, "dequeue: %p, %p, %i, %s\n",
		(void *)src, src->next, src->prio, (char *)&src->str);

	memset((void *)src, 0,  len);

	return 0;
}
