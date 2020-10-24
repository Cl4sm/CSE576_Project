int
tm_pstat(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	struct hostd *hp;
	struct waitc *wp;

	/* unpack and sanity check tid */

	if (upkuint(mp, &tid)) {
		pvmlogerror("tm_pstat() bad msg format\n");
		return 0;
	}
	if (!TIDISTASK(tid)) {
		pvmlogprintf("tm_pstat() bad tid %x\n", tid);
		return 0;
	}

	/* nack if no such host */

	if (!(hp = tidtohost(hosts, tid))) {
		mp = replymessage(mp);
		pkint(mp, PvmNoTask);
		sendmessage(mp);
		return 0;
	}

	/* else make a wait context and send query */

	wp = wait_new(WT_PSTAT);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp = mesg_new(0);
	mp->m_dst = hp->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_PSTAT;
	mp->m_wid = wp->wa_wid;
	pkint(mp, tid);
	sendmessage(mp);
	return 0;
}
