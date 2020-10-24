static int
pvm_tc_taskexit(mid)
	int mid;
{
	int src;					/* sender of notify */
	struct peer *pp;
	int found_one = 0;			/* did we find a peer? */

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);


	if (!outta_here) {
		int tid;
		extern struct peer *peers;

		pvm_upkint(&tid, 1, 1);

		for (pp = peers->p_link; pp != peers; pp = pp->p_link)
			if (pp->p_tid == tid) {
				pp->p_exited = 1;		/* mark as deleted */
				found_one = 1;
				break;
			}

	}

	if (!found_one) /* then it was unexpected! */
		pvmlogprintf("pvm_tc_taskexit() unexpected TC msg from t%x!\n", src);

	pvm_freebuf(mid);
	return 0;
}
