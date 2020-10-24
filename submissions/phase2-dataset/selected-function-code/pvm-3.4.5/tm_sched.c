tm_sched(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct hostd *hp;

	mp2 = replymessage(mp);
	pkint(mp2, pvmschedtid);
	hp = hosts->ht_hosts[hosts->ht_master];
	pkint(mp2, hp->hd_hostpart);
	pkstr(mp2, hp->hd_name);
	pkstr(mp2, hp->hd_arch ? hp->hd_arch : "");
	pkint(mp2, hp->hd_speed);
	pkint(mp2, hp->hd_dsig);
	sendmessage(mp2);

	if (pvmdebmask & PDMSCHED) {
		pvmlogprintf("tm_sched() old t%x new t%x\n", pvmschedtid, tp->t_tid);
	}
	pvmschedtid = tp->t_tid;
	tp->t_flag |= TF_ISSCHED;
	return 0;
}
