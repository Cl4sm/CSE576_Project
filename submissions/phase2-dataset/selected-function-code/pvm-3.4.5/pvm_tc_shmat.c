pvm_tc_shmat(mid)
	int mid;
{
	int src;					/* sender of request */
	struct peer *pp;
	struct msgboxhdr *dboxp;
	int ackd;					/* allow connection (0) */

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	if (!(pp = peer_conn(src, (int *)0)) || pp == (struct peer *)-1L) {
		pvmlogprintf("pvm_tc_shmat() can't connect to src t%x\n", src);
		ackd = -1;
	} else
		ackd = pvmmytid;
	dboxp = (struct msgboxhdr *)pp->p_buf;
	dboxp->mb_attached = ackd;

	pvm_freebuf(mid);
	return 0;
}
