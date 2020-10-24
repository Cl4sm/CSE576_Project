int
pvmbeatask()
{
	struct shmpkhdr *inmsgs;
	struct pidtidhdr *pvminfo;
	struct msgboxhdr *dboxp;		/* receiving box of pvmd */
	int next;
	int altpid;						/* pid of ancestor forked by pvmd */
	int msgcnt;
	char *msgbuf;					/* my message buffer */
	int bufid;
	int i;
	int pid;
	int bid;
	int rc;
	char *p;
	struct pvmminfo minfo;
	int cc = 0;
	int sbf = 0, rbf = 0;			/* saved rx and tx message handles */
	int outtid, outctx, outtag;
	int trctid, trcctx, trctag;
	int need_trcinfo = 0;
	int new_tracer = 0;
	char tmask[ 2 * TEV_MASK_LENGTH ];
	int tbuf, topt;
	int mid;
/*
	union semun {
		int val;
		struct semid_ds *buf;
		ushort *array;
	} sunion;
*/
#ifdef LOG
	char fname[32];
#endif
	int key, firstkeytried;
	int mytid;
	char *pvmtmp;
	TEV_DECLS

	if (pvmmytid != -1)
		return 0;

	pvmmydsig = pvmgetdsig();

	TEV_EXCLUSIVE;

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("pvmbeatask() can't getuid()\n");
		return PvmSysErr;
	}

	if (p = getenv("PVMTASKDEBUG")) {
		pvmdebmask = pvmxtoi(p);
		if (pvmdebmask) {
			sprintf(pvmtxt,"task debug mask is 0x%x\n", pvmdebmask);
			pvmlogerror(pvmtxt);
		}
	}

	pvmmyupid = getpid();

#ifdef LOG
#ifdef IMA_CSPP
	int scid = get_scid();

	pvmtmp = pvmgettmp();

	if (scid > 1)
		sprintf(fname, "%s/pvmt.%d.%d", pvmtmp, pvm_useruid, scid);
	else
#endif
		sprintf(fname, "%s/pvmt.%d", pvmtmp, pvm_useruid);
		if ((logfp = fopen(fname, "a")) == NULL)
			pvmlogerror("pvmbeatask() can't open log file\n");
#endif

	/*
	* get expected pid from environment in case we were started by
	* the pvmd and someone forked again
	*/

	if (p = getenv("PVMEPID"))
		altpid = atoi(p);
	else
		altpid = 0;

	pgsz = sysconf(_SC_PAGESIZE);
	pvmpgsz = FRAGPAGE*pgsz;
	pvmfrgsiz = pvmpgsz - PVMPAGEHDR;
	pvminboxsz =
		(INBOXPAGE*pgsz - sizeof(struct msgboxhdr))/sizeof(struct shmpkhdr);

	/*
	*	initialize received-message list and fragment reassembly list
	*/

	rxfrag = pmsg_new(1);
	BZERO((char*)rxfrag, sizeof(struct pmsg));
	rxfrag->m_link = rxfrag->m_rlink = rxfrag;

	pvmrxlist = pmsg_new(1);
	BZERO((char*)pvmrxlist, sizeof(struct pmsg));
	pvmrxlist->m_link = pvmrxlist->m_rlink = pvmrxlist;

	peer_init();

	/*
	* SIGTERM handler to clean up our shared memory
	*/

	if (pvmsettermhdlr) {
		pvmoldtermhdlr = signal(SIGTERM, catch_kill);
		pvmsettermhdlr = 0;
	}

	/*
	* get pvmd's message buffer, check protocol revision
	*/

	key = pvmshmkey(0);
	if ((bufid = shmget((key_t)key, 0, PERMS)) == -1) {
		pvmlogperror("pvmbeatask() shmget: can't connect to pvmd");
		return PvmSysErr;
	}
	if ((pvmdinbox = (char *)shmat(bufid, 0, 0)) == (char *)-1L) {
		pvmlogperror("pvmbeatask() shmat pvmd");
		return PvmSysErr;
	}
	infopage = pvmdinbox + INBOXPAGE*pgsz;
	pvmdoutbuf = infopage + pgsz;

	pvminfo = (struct pidtidhdr *)(infopage + PVMPAGEHDR);

	while ( read_int( (int *) &(pvminfo[0]) ) == 0 ) {
		pvmsleep(1);
		if (pvmdebmask & PDMMEM)
			pvmlogerror("Waiting for pvmd to set protocol\n");
	}
	if (pvminfo->i_proto != TDPROTOCOL) {
		sprintf(pvmtxt, "beatask() t-d protocol mismatch (%d/%d)\n",
			TDPROTOCOL, *((int *) &(pvminfo[0])));
		pvmlogerror(pvmtxt);
		return PvmSysErr;
	}

	/*
	* send it a request for connection/task assignment
	*/

	dboxp = (struct msgboxhdr *)pvmdinbox;
	inmsgs = (struct shmpkhdr *)(dboxp + 1);
	PAGELOCK(&dboxp->mb_lock);
	while ((next = (dboxp->mb_last + 1) % pvminboxsz) == dboxp->mb_read) ;
	/* XXX yuck, overloading these fields */
	inmsgs[next].ph_src = pvmmyupid;
	inmsgs[next].ph_dst = altpid;
	inmsgs[next].ph_dat = -1;
	inmsgs[next].ph_sdr = 0;
	inmsgs[next].ph_len = 0;
	inmsgs[next].ph_flag = 0;
	inmsgs[next].ph_tag = 0;
	inmsgs[next].ph_enc = 0;
	inmsgs[next].ph_wid = 0;
	inmsgs[next].ph_crc = 0;
	dboxp->mb_last = next;
	PAGEUNLOCK(&dboxp->mb_lock);

	pvmdpid = pvminfo->i_dpid;
	(void)getdsock();

	PAGELOCK(&dboxp->mb_lock);
	if ((next - 1) % pvminboxsz == dboxp->mb_read) {
		PAGEUNLOCK(&dboxp->mb_lock);
		(void)prodpvmd();
	} else
		PAGEUNLOCK(&dboxp->mb_lock);


	/*
	* get global parameters from pvmd buffer
	*/

/*
	pvmmydsig = pvminfo->i_dsig;
*/
	shmbufsiz = pvminfo->i_bufsiz;
	outbufsz = (shmbufsiz - INBOXPAGE*pgsz)/pvmpgsz;
	nbufsowned = 0;
	pidtids = (struct pidtid *)(pvminfo + 1);
	maxpidtid = (pgsz - sizeof(struct pidtidhdr) - PVMPAGEHDR)/sizeof(struct pidtid);

	/*
	* wait for pvmd to write us an entry in pidtid table
	*/

	pid = altpid ? altpid : pvmmyupid;
	mytid = -1;
	while (mytid == -1) {
		int ntids;			/* number of entries in pid-tid table */

		PAGELOCK(&((struct shmpghdr *)infopage)->pg_lock);
		ntids = min(maxpidtid, ((struct shmpghdr *)infopage)->pg_ref);
		for (i = 0; i < ntids; i++)
			if (pidtids[i].pt_pid == pid) {
				mytid = pidtids[i].pt_tid;
				pvmmyptid = pidtids[i].pt_ptid;
				/* pidtids[i].pt_pid = pvmmyupid; */
				mypidtid = i;
				break;
			}
		PAGEUNLOCK(&((struct shmpghdr *)infopage)->pg_lock);
		if (mytid==-1) { /* not entered in pvmd info page yet */
			prodpvmd();	/* kick daemon out of select() in work() */
			sleep_dammit( 10000 );	/* prevent peer page lock race */
									/* on info page */
		}
	}


	/*
	* create shared memory segment (and semaphore)
	* if we can't get the first key, keep trying others
	* XXX this could be moved back to after the protocol num check.
	*/

	firstkeytried = key = pvmshmkey(getpid());
	while (1) {

/* moved here from down there */
myshmbufid = -1;
errno = ENOSPC;
/* static int once = 0; */
/* if (once > 3) */
		myshmbufid = shmget((key_t)key, shmbufsiz, IPC_CREAT|IPC_EXCL|PERMS);
/* else { */
/* myshmbufid = -1; */
/* errno = ENOSPC; */
/* once++; */
/* } */
		if (myshmbufid == -1) {
			if (errno != EACCES && errno != EEXIST) {
				pvmlogperror("pvmbeatask() shmget");
				return PvmSysErr;
			}

		} else {
#ifdef	USERECVSEMAPHORE
			mysemid = semget((key_t)key, 1, IPC_CREAT|IPC_EXCL|PERMS);
			if (mysemid == -1) {
				if (errno != EACCES && errno != EEXIST) {
					pvmlogperror("pvmbeatask() semget");
					shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
					return PvmSysErr;

				} else {
					shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
				}

			} else {
/*
				sunion.val = 0;
				if (semctl(mysemid, 0, SETVAL, sunion) == -1) {
					pvmlogperror("pvmbeatask() semctl SETVAL");
					return PvmSysErr;
				}
*/
				break;
			}
#else
			break;
#endif
		}

		key = nextpvmshmkey(key);
		if (key == firstkeytried) {
			pvmlogerror("pvmbeatask() can't find a free key!\n");
			return PvmSysErr;
		}
	}

#ifdef IMA_CSPP
	if ((pvminbox = (char *)shm_search(myshmbufid)) == (char *)-1L)
#else
	if ((pvminbox = (char *)shmat(myshmbufid, 0, 0)) == (char *)-1L)
#endif
	{
		pvmlogperror("pvmbeatask() shmat");
		shmctl(myshmbufid, IPC_RMID, (struct shmid_ds *)0);
		return PvmSysErr;
	}

	outmsgbuf = pvminbox + INBOXPAGE*pgsz;
	msgbufinit(pvminbox);
	/* XXX PAGELOCK(pvminfo); */
	pidtids[mypidtid].pt_key = key;
	pidtids[mypidtid].pt_stat = ST_SHMEM;
#ifdef IMA_CSPP
	pidtids[mypidtid].pt_node = current_node();
#endif
	/* XXX PAGEUNLOCK(pvminfo); */

	pvmmytid = mytid;

	/* OK..
	 * Daemon has been prodded... so lets give it our Unix pid via
	 * the socket so it can match the socket to the process.
	 * That way, when the socket goes EOF pvmd knows which process
	 * it was....
	 * Note we call it this late so that pmsg buffers are available.
	 * Although the message is constructed in shmem its sent via the
	 * socket!
	 */

	send_my_pid();


	/*
	*	Request task trace/output paramters from daemon
	*/

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	rbf = pvm_setrbuf(0);
	if ((cc = msendrecv(TIDPVMD, TM_GETOPT, SYSCTX_TM)) > 0) {
		pvm_upkint(&rc, 1, 1);	/* throw out tid, ptid */
		pvm_upkint(&rc, 1, 1);

		pvm_upkint(&outtid, 1, 1);
		pvm_upkint(&outctx, 1, 1);
		pvm_upkint(&outtag, 1, 1);
		if (!pvmtrc.outtid) {
			pvmtrc.outtid = outtid;
			pvmtrc.outctx = outctx;
			pvmtrc.outtag = outtag;
			pvmctrc.outtid = pvmtrc.outtid;
			pvmctrc.outctx = pvmtrc.outctx;
			pvmctrc.outtag = pvmtrc.outtag;
		}

		pvm_upkint(&trctid, 1, 1);
		pvm_upkint(&trcctx, 1, 1);
		pvm_upkint(&trctag, 1, 1);
		if (!pvmtrc.trctid) {
			pvmtrc.trctid = trctid;
			pvmtrc.trcctx = trcctx;
			pvmtrc.trctag = trctag;
			pvmctrc.trctid = pvmtrc.trctid;
			pvmctrc.trcctx = pvmtrc.trcctx;
			pvmctrc.trctag = pvmtrc.trctag;
			new_tracer++;
		}

		cc = 0;
	}
	pvm_freebuf(pvm_setrbuf(rbf));
	pvm_freebuf(pvm_setsbuf(sbf));

	if (p = getenv("PVMCTX"))
		pvmmyctx = pvmstrtoi(p);

	/* get trace mask from envar or zero it */

	if ( (p = getenv("PVMTMASK")) ) {
		if ( strlen(p) + 1 == TEV_MASK_LENGTH )
			BCOPY(p, pvmtrc.tmask, TEV_MASK_LENGTH);
		else
			TEV_MASK_INIT(pvmtrc.tmask);
	} else {
		TEV_MASK_INIT(pvmtrc.tmask);
		if ( new_tracer ) need_trcinfo++;
	}

	BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);

	/* get trace buffering from envar */

	if ((p = getenv("PVMTRCBUF")))
		pvmtrc.trcbuf = atoi( p );
	else {
		pvmtrc.trcbuf = 0;
		if ( new_tracer ) need_trcinfo++;
	}

	pvmctrc.trcbuf = pvmtrc.trcbuf;

	/* get trace options from envar */

	if ((p = getenv("PVMTRCOPT")))
		pvmtrc.trcopt = atoi( p );
	else {
		pvmtrc.trcopt = 0;
		if ( new_tracer ) need_trcinfo++;
	}

	pvmctrc.trcopt = pvmtrc.trcopt;

	/* Setup waitc structures */
	wait_init(pvmmytid, TIDLOCAL);


	BZERO(&minfo, sizeof(minfo));
	minfo.src = -1;
	minfo.ctx = SYSCTX_TC;
	minfo.tag = TC_SHMAT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_shmat);
	minfo.tag = TC_CONREQ;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_conreq);
	minfo.tag = TC_CONACK;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_conack);
	minfo.tag = TC_TASKEXIT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_taskexit);
	minfo.tag = TC_NOOP;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_noop);
	minfo.tag = TC_SETTRACE;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrace);
	minfo.tag = TC_SETTRCBUF;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrcbuf);
	minfo.tag = TC_SETTRCOPT;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settrcopt);
	minfo.tag = TC_SETTMASK;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_settmask);
	minfo.tag = TC_SIBLINGS;
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvm_tc_siblings);

	if ( need_trcinfo )
	{
		rbf = pvm_setrbuf( 0 );

		if ( pvm_recvinfo( PVMTRACERCLASS, 0, PvmMboxDefault ) > 0 )
		{
			pvm_upkint(&trctid, 1, 1);

			pvm_upkint(&trcctx, 1, 1);
			pvm_upkint(&trctag, 1, 1);

			pvm_upkint(&outctx, 1, 1);  /* unused here */
			pvm_upkint(&outtag, 1, 1);  /* unused here */

			pvm_upkstr(tmask);

			pvm_upkint(&tbuf, 1, 1);
			pvm_upkint(&topt, 1, 1);

			if ( pvmtrc.trctid == trctid && pvmtrc.trcctx == trcctx
					&& pvmtrc.trctag == trctag )
			{
				if ( strlen(tmask) + 1 == TEV_MASK_LENGTH ) {
					BCOPY(tmask, pvmtrc.tmask, TEV_MASK_LENGTH);
					BCOPY(pvmtrc.tmask, pvmctrc.tmask, TEV_MASK_LENGTH);
				}

				pvmtrc.trcbuf = tbuf;
				pvmctrc.trcbuf = pvmtrc.trcbuf;

				pvmtrc.trcopt = topt;
				pvmctrc.trcopt = pvmtrc.trcopt;
			}

			pvm_freebuf(pvm_setrbuf(rbf));
		}

		else
			pvm_setrbuf(rbf);
	}

	tev_init();

	if (TEV_AMEXCL) {
		TEV_ENDEXCL;
	}

	return cc;
}
