tm_getopt(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	mp = replymessage(mp);
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
	mp->m_flag |= MM_PRIO;  /* set priority handling for this message */
	sendmessage(mp);

#if defined(SHMEM)

	/* now change connection to TF_SHMCONN and try send packets that have
		gathered in this tasks sendq */
	tp->t_flag &= ~TF_PRESHMCONN;  
	tp->t_flag |= TF_SHMCONN;
	
	shm_wrt_pkts(tp); /* write  any waiting packets */
	
#endif
	return 0;
}
