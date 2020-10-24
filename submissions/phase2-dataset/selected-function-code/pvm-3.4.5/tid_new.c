int
tid_new()
{
	static int lastind = 0;		/* last local-part assigned */

	int startind;				/* to detect when we're out of tids */
	struct task *tp;
	int tid;

	if (++lastind > tidlmask)
		lastind = 1;
	startind = lastind;
	tp = locltasks;

	while (1) {
		tid = myhostpart + lastind;
		while (tp->t_tid < tid)
			if ((tp = tp->t_link) == locltasks)
				break;

		if (tp->t_tid != tid)
			return tid;

		if (++lastind > tidlmask) {
			lastind = 1;
			tp = locltasks;
		}
		if (lastind == startind)
			return -1;
	}
}
