int
tm_mstat(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	char *name;
	struct hostd *hp;
	struct waitc *wp;

	if (upkstralloc(mp, &name)) {
		pvmlogerror("tm_mstat() bad msg format\n");
		return 0;
	}

	hp = nametohost(hosts, name);

	PVM_FREE(name);

	if (!hp) {
		mp = replymessage(mp);
		pkint(mp, PvmNoHost);
		sendmessage(mp);
		return 0;
	}

	wp = wait_new(WT_MSTAT);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp = mesg_new(0);
	mp->m_dst = hp->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_PSTAT;
	mp->m_wid = wp->wa_wid;
	pkint(mp, hp->hd_hostpart | TIDPVMD);
	sendmessage(mp);
	return 0;
}
