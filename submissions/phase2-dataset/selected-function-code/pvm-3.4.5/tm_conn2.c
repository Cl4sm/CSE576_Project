tm_conn2(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	int pid;						/* real pid of task */
	int cookie;						/* cookie to identify task or 0 */
	struct task *tp2;				/* to search for existing context */
	struct pmsg *mp2;
	int cc;
	char c;
	char *taskname = (char *) NULL;

	if (upkint(mp, &pid) || upkint(mp, &cookie)) {
		pvmlogerror("tm_conn2() bad msg format\n");
		goto bail;
	}
	if (!cookie)
		cookie = pid;

	if (!(tp->t_flag & TF_AUTH)) {
		pvmlogprintf("tm_conn2() message from t%x, TF_AUTH not set\n",
				tp->t_tid);
		return 0;
	}

	/*
	*	check that task could write d-auth file
	*/

#ifndef NOPROT

	if ((cc = read(tp->t_authfd, &c, 1)) == -1) {
		pvmlogperror("tm_conn2() can't read d-auth file");
		return 0;
	}

	if (cc != 1) {
		pvmlogerror("tm_conn2() task didn't validate itself\n");
		goto bail;
	}

	(void)close(tp->t_authfd);
	tp->t_authfd = -1;
	(void)unlink(tp->t_authnam);

#endif

	PVM_FREE(tp->t_authnam);
	tp->t_authnam = 0;

	/*
	*	if task spawned by us, already has a context,
	*	else make it one
	*/

	if ((tp2 = task_findpid(cookie)) && !(tp2->t_flag & (TF_AUTH|TF_CONN))) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("tm_conn2() reconnect task t%x\n", tp2->t_tid);
		}
		tp->t_sched = tp2->t_sched;

#ifdef IMA_BEOLIN
	} else if ( (tp2 = mpp_find(tp))
			&& !(tp2->t_flag & (TF_AUTH|TF_CONN)) ) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf(
					"tm_conn2() reconnect task t%x pid=%d name=%s\n",
					tp2->t_tid,tp2->t_pid, tp2->t_a_out );
		}
#endif

	} else {
		if ((tid = tid_new()) < 0) {
			pvmlogerror("tm_conn2() out of tids?\n");
			goto bail;		/* XXX should disconnect nicely */
		}
		if ((tp2 = task_new(tid)) == NULL) {
			pvmlogerror("tm_conn2() too many tasks?\n");
			goto bail;		/* XXX should disconnect nicely */
		}

		if ( !upkstralloc(mp, &taskname) ) {
			tp2->t_a_out = taskname;
		}

		if (pvmschedtid) {
			/* inform the scheduler about the new task */

			tp->t_sched = pvmschedtid;
			mp2 = mesg_new(0);
			pkint(mp2, 1);
			pkint(mp2, tid);
			mp2->m_tag = SM_EXECACK;		/* XXX yecch, overload */
			mp2->m_dst = pvmschedtid;
			sendmessage(mp2);
		}

		/* check for tracer */
		if (pvmtracer.trctid && pvmtracer.trctag) {
			tp2->t_trctid = pvmtracer.trctid;
			tp2->t_trcctx = pvmtracer.trcctx;
			tp2->t_trctag = pvmtracer.trctag;
			/* Note:  can't get the trace mask or trace buffer size
				to task here, but this will be handled using mailbox */

			/* trace start message */
			/* XXX Note:  we don't really know if we should send this
				because this task could have prohibited tracing
				by setting PvmSelfTraceTid to -1... */
			tev_send_newtask(
				pvmtracer.trctid, pvmtracer.trcctx, pvmtracer.trctag,
				tid, -1, -1, tp->t_a_out ? tp->t_a_out : "-" );
		}
		if (pvmtracer.outtid && pvmtracer.outtag) {
			tp2->t_outtid = pvmtracer.outtid;
			tp2->t_outctx = pvmtracer.outctx;
			tp2->t_outtag = pvmtracer.outtag;

			/* output start message */
			/* Note:  same deal here as for TEV_NEWTASK above... */
			mp2 = mesg_new(0);
			mp2->m_dst = pvmtracer.outtid;
			mp2->m_ctx = pvmtracer.outctx;
			mp2->m_tag = pvmtracer.outtag;
			pkint(mp2, tid);
			pkint(mp2, TO_NEW);
			pkint(mp2, -1);
			sendmessage(mp2);
		}

		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("tm_conn2() new task t%x\n", tp2->t_tid);
		}
	}

	/*
	*	brundle-fly the contexts together
	*/

	tp2->t_sock = tp->t_sock;
	tp2->t_sad = tp->t_sad;
	tp2->t_salen = tp->t_salen;

#ifdef NOPROT
	if (!TIDISNODE(tp2->t_tid)) {
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf( "Setting pid of t%x to %d, same as t%x\n",
					tp2->t_tid, tp->t_pid, tp->t_tid );
		}
		task_setpid(tp2, tp->t_pid);
	}
#else
	if (tp2->t_pid != pid)
		task_setpid(tp2, pid);
#endif

	tp2->t_rxp = tp->t_rxp;
	tp2->t_sched = tp->t_sched;
	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp = tp2;
	if (cookie != pid)
		tp->t_flag &= ~TF_FORKD;

	/*
	*	kick it in the butt; it's ready to go
	*/

	tp->t_flag &= ~TF_AUTH;
	tp->t_flag |= TF_CONN;

	mp = replymessage(mp);
	pkint(mp, 1);
	pkint(mp, tp->t_tid);
	pkint(mp, tp->t_ptid);
	pkint(mp, tp->t_outtid);
	pkint(mp, tp->t_outctx);
	pkint(mp, tp->t_outtag);
	pkint(mp, tp->t_trctid);
	pkint(mp, tp->t_trcctx);
	pkint(mp, tp->t_trctag);
	pkint(mp, pvmudpmtu);
	pkint(mp, pvmmydsig);
	pkstr(mp, inadport_hex(&(hosts->ht_hosts[hosts->ht_local]->hd_sad)));
	pkint(mp, pvmschedtid);
	mp->m_dst = tp->t_tid;
	mp->m_flag |= MM_PRIO;
	sendmessage(mp);
	return 0;

bail:
	tp->t_flag |= TF_CLOSE;
	return 0;
}
