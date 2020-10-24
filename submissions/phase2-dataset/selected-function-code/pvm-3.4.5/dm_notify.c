int
dm_notify(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int what, tid;
	struct waitc *wp;
	struct pmsg *mp2;

	hp = hp;

	upkint(mp, &what);
	upkuint(mp, &tid);
	if (what != PvmTaskExit) {
		pvmlogprintf("dm_notify() what = %d?\n", what);
		return 0;
	}

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_NOTIFYACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);

	if (task_find(tid)) {
		wp = wait_new(WT_TASKX);
		wp->wa_on = tid;
		wp->wa_tid = mp->m_src;
		wp->wa_dep = mp->m_wid;
		wp->wa_mesg = mp2;

	} else {
		sendmessage(mp2);
	}
	return 0;
}
