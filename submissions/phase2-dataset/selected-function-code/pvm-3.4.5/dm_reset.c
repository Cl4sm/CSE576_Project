dm_reset(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	struct waitc *wp;
	struct pmsg *mp2;

	hp = hp;

	upkuint(mp, &tid);

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_RESETACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);

	if (task_find(tid)) {
		wp = wait_new(WT_RESET);
		wp->wa_on = tid;
		wp->wa_tid = mp->m_src;
		wp->wa_dep = mp->m_wid;
		wp->wa_mesg = mp2;

	} else {
		sendmessage(mp2);
	}

	return 0;
}
