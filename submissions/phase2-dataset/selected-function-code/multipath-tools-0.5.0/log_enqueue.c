{
	int len, fwd;
	char buff[MAX_MSG_SIZE];
	struct logmsg * msg;
	struct logmsg * lastmsg;

	lastmsg = (struct logmsg *)la->tail;

	if (!la->empty) {
		fwd = sizeof(struct logmsg) +
		      strlen((char *)&lastmsg->str) * sizeof(char) + 1;
		la->tail += ALIGN(fwd, sizeof(void *));
	}
	vsnprintf(buff, MAX_MSG_SIZE, fmt, ap);
	len = ALIGN(sizeof(struct logmsg) + strlen(buff) * sizeof(char) + 1,
		    sizeof(void *));

	/* not enough space on tail : rewind */
	if (la->head <= la->tail && len > (la->end - la->tail)) {
		logdbg(stderr, "enqueue: rewind tail to %p\n", la->tail);
		if (la->head == la->start ) {
			logdbg(stderr, "enqueue: can not rewind tail, drop msg\n");
			la->tail = lastmsg;
			return 1;  /* can't reuse */
		}
		la->tail = la->start;

		if (la->empty)
			la->head = la->start;
	}

	/* not enough space on head : drop msg */
	if (la->head > la->tail && len >= (la->head - la->tail)) {
		logdbg(stderr, "enqueue: log area overrun, drop msg\n");

		if (!la->empty)
			la->tail = lastmsg;

		return 1;
	}

	/* ok, we can stage the msg in the area */
	la->empty = 0;
	msg = (struct logmsg *)la->tail;
	msg->prio = prio;
	memcpy((void *)&msg->str, buff, strlen(buff) + 1);
	lastmsg->next = la->tail;
	msg->next = la->head;

	logdbg(stderr, "enqueue: %p, %p, %i, %s\n", (void *)msg, msg->next,
		msg->prio, (char *)&msg->str);

#if LOGDBG
	dump_logarea();
#endif
	return 0;
}
