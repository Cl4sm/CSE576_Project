{
	static int lastpinged = 0;	/* host that got last keepalive msg */
#ifdef FDSETNOTSTRUCT
	fd_set rfds, wfds;			/* result of select */
	/* fd_set efds; */
#else
	struct fd_set rfds, wfds;	/* result of select */
	/* struct fd_set efds; */
#endif
	int nrdy;					/* number of fds ready after select */
	struct timeval tbail;		/* time to bail if state = STARTUP */
	struct timeval tping;		/* time to send next keepalive packet */
	struct timeval tnow;
	struct timeval tout;
	struct pmsg *mp;
	struct task *tp;
	struct hostd *hp;
#if defined(IMA_PGON) || defined(IMA_I860) || defined(SHMEM)
	int nodemsg = 0;
#if defined(IMA_PGON)
	struct timeval tpgon;		/* time to spend in paragon select */
	double tbpl;				/* time at beginning of probe loop */
	double toutpl;				/* timeout in the probe loop */
	int timed_out;
	extern double dclock();
#endif
#endif
#ifdef	SHMEM
	int someclosed;
#endif

	gettimeofday(&tnow, (struct timezone*)0);
	if (pvmdebmask || myhostpart) {
		PVM_TIMET time_temp;
		pvmlogprintf("%s (%s) %s %s\n",
				hosts->ht_hosts[hosts->ht_local]->hd_name,
				inadport_decimal(&hosts->ht_hosts[hosts->ht_local]->hd_sad),
				myarchname,
				PVM_VER);
		pvmlogprintf("ready ");
		time_temp = (PVM_TIMET) tnow.tv_sec;
		pvmlogprintf(ctime(&time_temp));
	}

	/*
	* check for default plug-in modules (& start them)
	* (only if pvmd, not for pvmd'...)
	*/

	if ( myhostpart ) {
		char *av[5];
		char *buf;
		char *cmd;
		int ac;
		int i;

		for ( i=0 ; modulenames[i] ; i++ ) {
			cmd = getenv( modulenames[i] );
			if ( cmd != NULL )
			{
				buf = STRALLOC( cmd );
				ac = sizeof(av)/sizeof(av[0]);
				if (!buf || acav( buf, &ac, av )) {
					pvmlogprintf( "$%s: Line Too Long \"%s\".\n",
							modulenames[i], cmd );
				} else {
					av[ ac ] = (char *) NULL;
					locl_spawn( av[0], av + 1 );
				}
				if (buf) PVM_FREE( buf );
			}
		}
	}

	/*
	* remind myself to start those pesky slave pvmds
	*/

	if (addmesg) {
		struct pmsg *mp = addmesg;

		addmesg = 0;
		sendmessage(mp);
	}

	/*
	* init bail (for PVMDSTARTUP) and ping times
	*/

	pvmgetclock(&tnow);
	tout.tv_sec = DDBAILTIME;
	tout.tv_usec = 0;
	TVXADDY(&tbail, &tnow, &tout);

	tout.tv_sec = DDPINGTIME;
	tout.tv_usec = 0;
	TVXADDY(&tping, &tnow, &tout);

	/* init select fd sets */

	wrk_fds_init();

	if (loclsock >= 0)
		wrk_fds_add(loclsock, 1);
	wrk_fds_add(netsock, 1);

	for (; ; ) {

		/*
		*	clean up after any tasks that we got SIGCHLDs for
		*/
#ifdef IMA_BEOSCYLD
		reap(SIGCLD);
#endif
		while (rdead != wdead) {
			if (deads[rdead].dd_pid == pprime) {
				int cc;
#ifdef SOCKLENISUINT
#if defined(IMA_AIX4SP2) || defined(IMA_AIX5SP2) \
		|| defined(IMA_AIX56K64) || defined(IMA_LINUXALPHA)
				unsigned int oslen;
#else
				size_t oslen;
#endif
#else
				int oslen;
#endif
				struct sockaddr_in osad;
				struct timeval t;
				char buf[DDFRAGHDR];

				hostfailentry(hosts->ht_hosts[0]);
				clear_opq_of((int)(TIDPVMD | hosts->ht_hosts[0]->hd_hostpart));
				pprime = 0;

				while (1) {
					FD_ZERO(&rfds);
					FD_SET(ppnetsock, &rfds);
					t.tv_sec = 0;
					t.tv_usec = 0;
					cc = select(ppnetsock + 1,
#ifdef	FDSETISINT
							(int *)&rfds, (int *)0, (int *)0,
#else
							(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
							&t);
					if (cc == 1) {
						oslen = sizeof(osad);
						recvfrom(ppnetsock, buf, sizeof(buf),
								0, (struct sockaddr*)&osad, &oslen);

					} else if (cc != -1 || errno != EINTR)
						break;
				}

			} else {
				if (tp = task_findpid(deads[rdead].dd_pid)) {

		/* check for output one last time
		   XXX this could be cleaner by going through main select again
		   XXX before flushing the task */

					tp->t_status = deads[rdead].dd_es;
					tp->t_utime = deads[rdead].dd_ut;
					tp->t_stime = deads[rdead].dd_st;
					while (tp->t_out >= 0) {

#ifdef FDSETNOTSTRUCT
						fd_set rfds;
#else
						struct fd_set rfds;
#endif

						FD_ZERO(&rfds);
						FD_SET(tp->t_out, &rfds);
						TVCLEAR(&tout);
						if (select(tp->t_out + 1,
#ifdef	FDSETISINT
								(int *)&rfds, (int *)0, (int *)0,
#else
								(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
								&tout) == 1)
							loclstout(tp);

						else
							break;
					}
#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
					mpp_free(tp);
#endif
					task_cleanup(tp);
					task_free(tp);
				}
			}
			if (++rdead >= ndead)
				rdead = 0;
		}

		netoutput();

		if (runstate == PVMDHALTING) {
			pvmlogerror("work() pvmd halting\n");
			pvmbailout(0);
		}

		/* bail if new slave and haven't been configured for too long */
		pvmgetclock(&tnow);
		if (runstate == PVMDSTARTUP && TVXLTY(&tbail, &tnow)) {
			pvmlogerror("work() run = STARTUP, timed out waiting for master\n");
			pvmbailout(0);
		}

		/*
		* send keepalive message to remote pvmd once in a while
		*/
		if (TVXLTY(&tping, &tnow)) {
			if (pvmdebmask & (PDMPACKET|PDMSELECT))
				pvmlogerror("work() ping timer\n");
			if (runstate == PVMDNORMAL || runstate == PVMDHTUPD) {
				do {
					if (++lastpinged > hosts->ht_last)
						lastpinged = 1;
				} while (!(hp = hosts->ht_hosts[lastpinged]));

				if (hp->hd_hostpart != myhostpart
				&& hp->hd_txq->pk_link == hp->hd_txq) {
					mp = mesg_new(0);
					mp->m_tag = DM_NULL;
					mp->m_dst = hp->hd_hostpart | TIDPVMD;
					sendmessage(mp);
				}
			}
			tout.tv_sec = DDPINGTIME;
			tout.tv_usec = 0;
			TVXADDY(&tping, &tnow, &tout);
		}

		/*
		* figure select timeout
		*/

		if (opq->pk_tlink == opq)
			tout = tping;
		else
			tout = opq->pk_tlink->pk_rtv;

		if (TVXLTY(&tout, &tnow)) {
			TVCLEAR(&tout);

		} else {
			TVXSUBY(&tout, &tout, &tnow);
		}

		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() select tout is %d.%06d\n",
					tout.tv_sec, tout.tv_usec);
		}


#ifdef SHMEM
		if ((nodemsg = mpp_probe()) == 1) {
			mpp_input();
			TVCLEAR(&tout);
		}
#endif

		rfds = wrk_rfds;
		wfds = wrk_wfds;
/*
		efds = wrk_efds;
*/
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() wrk_nfds=%d\n", wrk_nfds);
			print_fdset("work() rfds=", wrk_nfds, &rfds);
			print_fdset("work() wfds=", wrk_nfds, &wfds);
		}

#if !defined(IMA_PGON) && !defined(IMA_I860)

		if ((nrdy = select(wrk_nfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)&wfds, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)&wfds, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
				pvmlogperror("work() select");
				pvmlogprintf(" wrk_nfds=%d\n", wrk_nfds);
				print_fdset(" rfds=", wrk_nfds, &wrk_rfds);
				print_fdset(" wfds=", wrk_nfds, &wrk_wfds);
				pvmlogprintf(" netsock=%d, ppnetsock=%d, loclsock=%d\n",
						netsock, ppnetsock, loclsock);
				task_dump();
				pvmbailout(0);
			}
		}

#else /*IMA_PGON/IMA_I860*/

		timed_out = 0;
		toutpl = (double)tout.tv_sec + 1e-6*((double)tout.tv_usec);
		tbpl = dclock();   /* use the pgon hw clock */

		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf(
					"work() probe loop timeout is %f dclock is %f\n",
					toutpl,tbpl);
		}

		do {
			totprobes++;
			if ((nodemsg = mpp_input()) > 1)
			{
				mpp_input();
				TVCLEAR(&tpgon);

			} else {
				tout.tv_sec = 0;
				tout.tv_usec = TIMEOUT;
			}
			rfds = wrk_rfds;
			wfds = wrk_wfds;

			nrdy = 0;

			if (totprobes % altprobe == 0)
			{
				totprobes = 0;

				if ((nrdy = select(wrk_nfds,
#ifdef	FDSETISINT
						(int *)&rfds, (int *)&wfds, (int *)0,
#else
						(fd_set *)&rfds, (fd_set *)&wfds, (fd_set *)0,
#endif
						&tout))
				== -1) {
					if (errno != EINTR) {
						pvmlogperror("work() select");
						pvmbailout(0);
					}
				}

				if ( (dclock() - tbpl ) >= toutpl)
					timed_out = 1;
				else
					timed_out = 0;
				if (timed_out && pvmdebmask & PDMSELECT)
				{
					pvmlogprintf(
							"work() probe loop timeout, dclock %f\n",
							dclock());
				}
			}

			/* Try to send packets that are still on the mpp output q */
			mpp_output( (struct task *) NULL, (struct pkt *) NULL);

		} while(!(nrdy || nodemsg || timed_out));

#endif /*IMA_PGON/IMA_I860*/

#ifdef	STATISTICS
		switch (nrdy) {
		case -1:
			stats.selneg++;
			break;
		case 0:
			stats.selzer++;
			break;
		default:
			stats.selrdy++;
			break;
		}
#endif
		if (pvmdebmask & PDMSELECT) {
			pvmlogprintf("work() SELECT returns %d\n", nrdy);
		}

	/*
	*	check network socket and local master socket for action
	*/

		if (nrdy > 0) {
			if (FD_ISSET(netsock, &rfds)) {
				nrdy--;
				netinput();
			}
			if (loclsock >= 0 && FD_ISSET(loclsock, &rfds)) {
				nrdy--;
				loclconn();
			}
		}

	/*
	*	check tasks for action
	*/

#ifdef	SHMEM
		someclosed = 0;
#endif
		if (loclsock >= 0) {
			for (tp = locltasks->t_link;
					nrdy > 0 && tp != locltasks;
					tp = tp->t_link) {

				if (tp->t_sock >= 0 && FD_ISSET(tp->t_sock, &rfds)) {
					FD_CLR(tp->t_sock, &rfds);
					nrdy--;
					if (loclinput(tp)) {
#ifdef	SHMEM
						if (tp->t_tid == 0)
							someclosed++;
#endif
						if (pvmdebmask & PDMTASK) {
							pvmlogprintf(
									"work() error reading from t%x, marking dead\n",
									tp->t_tid);
						}
						if (!(tp->t_flag & TF_FORKD)) {
							tp = tp->t_rlink;
							task_cleanup(tp->t_link);
							task_free(tp->t_link);

						} else
							wrk_fds_delete(tp->t_sock, 3);
						continue;
					}
				}

				if (tp->t_sock >= 0 && FD_ISSET(tp->t_sock, &wfds)) {
					FD_CLR(tp->t_sock, &wfds);
					nrdy--;
					if (locloutput(tp)) {
#ifdef	SHMEM
						if (tp->t_tid == 0)
							someclosed++;
#endif
						if (!(tp->t_flag & TF_FORKD)) {
							tp = tp->t_rlink;
							task_cleanup(tp->t_link);
							task_free(tp->t_link);

						} else
							wrk_fds_delete(tp->t_sock, 3);
						continue;
					}
				}

				if (tp->t_out >= 0 && FD_ISSET(tp->t_out, &rfds)) {
					FD_CLR(tp->t_out, &rfds);
					nrdy--;
					loclstout(tp);
				}
			}
		}
#if defined(IMA_CM5) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
	|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
		mpp_output((struct task *)0, (struct pkt *)0);
#endif
#ifdef	SHMEM
		if (someclosed)
			mpp_dredge();
#endif
	}
}
