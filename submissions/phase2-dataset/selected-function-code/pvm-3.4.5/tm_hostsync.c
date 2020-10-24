int
tm_hostsync(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int where;
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp;

	if (upkuint(mp, &where)) {
		pvmlogerror("tm_hostsync() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	if (!(hp = tidtohost(hosts, where))) {
		pkint(mp2, PvmNoHost);
		sendmessage(mp2);
		return 0;
	}

	wp = wait_new(WT_HOSTSYNC);
	wp->wa_tid = mp->m_src;
	wp->wa_on = hp->hd_hostpart;
	wp->wa_mesg = mp2;

	mp2 = mesg_new(0);
	mp2->m_dst = where | TIDPVMD;
	mp2->m_tag = DM_HOSTSYNC;
	mp2->m_wid = wp->wa_wid;

	sendmessage(mp2);
	return 0;
}
