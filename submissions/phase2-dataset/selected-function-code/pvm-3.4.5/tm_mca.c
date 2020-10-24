int
tm_mca(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct mca *mcap;			/* mca descriptor */
	int ndst;					/* num of dst tids */
	int *dsts;					/* dst tids */
	int tid;
	int i, j;

	/*
	* unpack list of dst tids from message (expect it to be sorted).
	* discard tids to nonexistent foreign hosts.
	*/

	mcap = mca_new();
	mcap->mc_tid = TIDGID | tp->t_tid;
	upkint(mp, &ndst);
	dsts = TALLOC(ndst, int, "dsts");
	for ( i=0, j=0 ; i < ndst ; i++ ) {
		upkuint(mp, &tid);
		if ((tid & tidhmask) == myhostpart || tidtohost(hosts, tid))
			dsts[j++] = tid;
	}
	ndst = j;

	if (ndst < 1)
		goto noguys;

	/*
	* send DM_MCA messages containing tids to destination hosts
	* make list of destination hosts for us
	*/

	mcap->mc_dsts = TALLOC(ndst, int, "mcal");	/* XXX cheat, too much space */
	mcap->mc_ndst = 0;

	for (j = 0; j < ndst; ) {
		i = j;
		tid = dsts[i] & tidhmask;
		while (++j < ndst && tid == (dsts[j] & tidhmask)) ;
		mp = mesg_new(0);
		mp->m_dst = (tid |= TIDPVMD);
		mp->m_tag = DM_MCA;
		pkint(mp, mcap->mc_tid);
		pkint(mp, j - i);
		while (i < j)
			pkint(mp, dsts[i++]);
		sendmessage(mp);
		mcap->mc_dsts[mcap->mc_ndst++] = tid;
	}

noguys:
	PVM_FREE(dsts);

	/*
	* tag task descriptor with mca desc and send mca back to task
	*/

	tp->t_mca = mcap;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf("tm_mca() made mca %x for t%x\n",
				mcap->mc_tid, tp->t_tid);
	}
	return 0;
}
