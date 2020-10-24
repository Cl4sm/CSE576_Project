task_free(tp)
	struct task *tp;
{
	struct timeval now;
	struct pmsg *mp;
	struct ccon *cp;

	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("task_free() t%x\n", tp->t_tid);
	}
#ifdef SHMEM
	/* XXX this is inside out - mpp_free should call task_free.
	   XXX but for now task_free is what's called.
	   XXX this will change in the portable processor interface
	   XXX cleanup. */
	mpp_free(tp->t_tid);
#endif
#ifdef IMA_BEOLIN
	mpp_free(tp);
#endif
	if (tp->t_plink && tp->t_prlink) {
		LISTDELETE(tp, t_plink, t_prlink);
	}
	if (tp->t_link && tp->t_rlink) {
		LISTDELETE(tp, t_link, t_rlink);
	}
	if (tp->t_rxm)
		pmsg_unref(tp->t_rxm);
	if (tp->t_rxp)
		pk_free(tp->t_rxp);
	if (tp->t_txq)
		pk_free(tp->t_txq);
	if (tp->t_wait)
		wait_delete(tp->t_wait);
	if (tp->t_authnam) {
		(void)unlink(tp->t_authnam);
		PVM_FREE(tp->t_authnam);
	}
	if (tp->t_sock != -1) {
		wrk_fds_delete(tp->t_sock, 3);
#ifdef WIN32
		closesocket(tp->t_sock);
#else
		(void)close(tp->t_sock);
#endif
	}
	if (tp->t_out != -1) {
		wrk_fds_delete(tp->t_out, 1);
#ifdef WIN32
		closesocket(tp->t_out);
#else
		(void)close(tp->t_out);
#endif
	}
	if (tp->t_outtid > 0) {
		mp = mesg_new(0);
		mp->m_dst = tp->t_outtid;
		mp->m_ctx = tp->t_outctx;
		mp->m_tag = tp->t_outtag;
		pkint(mp, tp->t_tid);
		pkint(mp, TO_EOF);
		sendmessage(mp);
		tp->t_outtid = 0;
	}
	if (tp->t_trctid > 0) {
		tev_send_endtask( tp->t_trctid, tp->t_trcctx, tp->t_trctag,
			tp->t_tid, tp->t_status,
			tp->t_utime.tv_sec, tp->t_utime.tv_usec,
			tp->t_stime.tv_sec, tp->t_stime.tv_usec );
		tp->t_trctid = 0;
	}
	if (tp->t_authfd != -1)
		(void)close(tp->t_authfd);
	if (tp->t_a_out)
		PVM_FREE(tp->t_a_out);
	if (tp->t_mca)
		mca_free(tp->t_mca);
	if (tp->t_ccs) {
		while (cp = LISTFIRST(tp->t_ccs, c_peer)) {
			LISTDELETE(cp, c_link, c_rlink);
			LISTDELETE(cp, c_peer, c_rpeer);
			PVM_FREE(cp);
		}
		PVM_FREE(tp->t_ccs);
	}

	if (tp->t_tid) {
		ntasks--;
		if (pvmdebmask & PDMTASK) {
			pvmlogprintf("task_free(%d) ntasks=%d\n",
					tp->t_tid, ntasks );
		}
	}

	PVM_FREE(tp);
}
