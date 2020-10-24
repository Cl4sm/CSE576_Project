{
	struct htab *htp;
	struct task *tp;
	int i;

	runstate = PVMDPRIME;

	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("beprime() can't getpid()\n");
		pvmbailout(0);
	}

	myhostpart = 0;
	pvmmytid = TIDPVMD;

	htp = ht_new(hosts->ht_local);
	htp->ht_master = hosts->ht_local;
	htp->ht_local = 0;
	ht_insert(htp, hosts->ht_hosts[hosts->ht_local]);
	ht_insert(htp, hosts->ht_hosts[0]);
	htp->ht_hosts[htp->ht_master]->hd_txseq
			= htp->ht_hosts[0]->hd_rxseq;
	htp->ht_hosts[htp->ht_master]->hd_rxseq
			= htp->ht_hosts[0]->hd_txseq;

	oldhosts = hosts;
	hosts = htp;

#ifndef NOUNIXDOM
	loclspath = 0;
#endif
	(void)close(loclsock);
	loclsock = -1;
	loclsnam = 0;
	(void)close(netsock);
	netsock = ppnetsock;
	ppnetsock = -1;

	locltasks = 0;
	task_init();

	/* close everything but netsock, log_fd and 0, 1, 2 */

	for (i = getdtablesize(); --i > 2; )
		if (i != netsock && i != log_fd)
			(void)close(i);

	wrk_fds_init();
	wrk_fds_add(netsock, 1);

	opq = pk_new(0);
	opq->pk_tlink = opq->pk_trlink = opq;

	wdead = 0;
	rdead = 0;

	return 0;
}
