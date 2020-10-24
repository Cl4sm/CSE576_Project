ppi_init()
{
	struct pidtidhdr *pvminfo;
	char *pvmtmp = pvmgettmp();
	char *p;
	int key;

#ifdef LOG
	char fname[32];
	FILE *logfp;
#ifdef IMA_CSPP
	int scid = get_scid();
	if (scid > 1)
		sprintf(fname, "%s/pvmt.%d.%d", pvmtmp, pvm_useruid, scid);
	else
#else
	sprintf(fname, "%s/pvmt.%d", pvmtmp, pvm_useruid);
#endif
	logfp = fopen(fname, "w");
	fclose(logfp);
#endif	/*LOG*/

	pvmpgsz = FRAGPAGE*pgsz;
	inboxsz =
		(INBOXPAGE*pgsz - sizeof(struct msgboxhdr))/sizeof(struct shmpkhdr);
	if (!(p = getenv("PVMBUFSIZE")) || !(shmbufsiz = strtol(p, (char**)0, 0)))
		shmbufsiz = SHMBUFSIZE;

	key = pvmshmkey(0);
	if ((myshmbufid = shmget((key_t)key, shmbufsiz, IPC_CREAT|PERMS)) == -1) {
		pvmlogperror("ppi_init() can't create msg buffer");
		pvmbailout(0);
		return;
	}
#ifdef IMA_CSPP
	if ((pvminbox = (char *)shm_search(myshmbufid)) == (char *)-1L)
#else
	if ((pvminbox = (char *)shmat(myshmbufid, 0, 0)) == (char *)-1L)
#endif
	{
		pvmlogperror("ppi_init() can't attach msg buffer");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		myshmbufid = -1;
		pvmbailout(0);
		return;
	}

	infopage = pvminbox + INBOXPAGE*pgsz;
	outmsgbuf = infopage + pgsz;
	if (!(outbufsz = (shmbufsiz - INBOXPAGE*pgsz - pgsz)/pvmpgsz)) {
		pvmlogerror("ppi_init() SHMBUFSIZE too small!");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		myshmbufid = -1;
		pvmbailout(0);
		return;
	}
	nbufsowned = 0;

	msgbufinit(pvminbox);
#ifndef IMA_KSR1
	PAGEINITLOCK(&((struct shmpghdr *)infopage)->pg_lock);
#endif
	((struct shmpghdr *)infopage)->pg_ref = 0;
	pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);
	pvminfo->i_proto = TDPROTOCOL;
	pvminfo->i_dsig = pvmmydsig;
	pvminfo->i_next = 0;
	pvminfo->i_bufsiz = shmbufsiz;
	pvminfo->i_dpid = pvmmyupid;
	pidtids = (struct pidtid *)(pvminfo + 1);
	maxpidtid = (pgsz - sizeof(struct pidtidhdr) - PVMPAGEHDR)/sizeof(struct pidtid);
	BZERO((char *)pidtids, sizeof(struct pidtid)*maxpidtid);
	{
		int i;

		for (i = 0; i < maxpidtid; i++)
			pidtids[i].pt_stat = ST_EXIT;
	}

	peer_init();

	ovfpkts = pk_new(0);
	ovfpkts -> pk_link = ovfpkts->pk_rlink = ovfpkts;

/*
#if defined(SUN4SOL2)
	sprintf(pvmtxt, "%ld CPUs online\n", sysconf(_SC_NPROCESSORS_ONLN));
	sprintf(pvmtxt, "%ld CPUs online\n", sysconf(15));
	pvmlogerror(pvmtxt);
#endif
*/
}
