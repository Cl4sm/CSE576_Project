pvmendtask()
{
	int i;
	struct shmid_ds shmds;
	struct pmsg *up;
	int sbf;
	static struct timeval ztv = { 0, 0 };
	struct peer *pp;
	extern struct peer *peers;
	struct pmsg *mp;

	/*
	* free any left-over messages.
	*/

	pvmsbuf = 0;
	pvmrbuf = 0;
	for (i = 1; i < pvmmidhsiz; i++)
		if (up = pvmmidh[i].m_umb)
			umbuf_free(up);

	/*
	* notify all connected tasks that we are exiting
	*/

	outta_here = 1;

	if (peers) {
		sbf = pvm_initsend(PvmDataFoo);
		mp = midtobuf(sbf);
		mp->m_ctx = SYSCTX_TC;
		pvm_pkint(&pvmmytid, 1, 1);

		for (pp = peers->p_link; pp != peers; pp = pp->p_link)
			if (pp->p_tid && pp->p_tid != pvmmytid)
				mroute(sbf, pp->p_tid, TC_TASKEXIT, &ztv);

		pvm_freebuf(sbf);
	}

	if (pidtids) {
		PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
		pidtids[mypidtid].pt_stat = ST_EXIT;
		PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
	}

	shmdt(pvminbox);
	if (shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0) == -1)
		pvmlogperror("pvmendtask() shmctl RMID");
#ifdef USERECVSEMAPHORE
	if (semctl(mysemid, 0, IPC_RMID) == -1)
		pvmlogperror("pvmendtask() semctl RMID");
	mysemid = -1;
#endif
	pvminbox = 0;
	shmdt(pvmdinbox);
	pvmdinbox = 0;
	infopage = 0;
	pvmdoutbuf = 0;
	pidtids = 0;
	mypidtid = -1;

	peer_cleanup();

	pvmmytid = -1;
	if (pvmdsock != -1) {
		(void)close(pvmdsock);
		pvmdsock = -1;
	}

	/* XXX free rxfrag and rxlist */
#ifdef LOG
	fclose(logfp);
#endif

	return 0;
}
