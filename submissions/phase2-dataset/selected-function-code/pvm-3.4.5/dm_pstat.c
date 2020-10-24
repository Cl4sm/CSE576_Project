dm_pstat(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int tid;
	struct pmsg *mp2;

	hp = hp;
	upkuint(mp, &tid);
	if (tid == pvmmytid || task_find(tid))
		tid = 0;
	else
		tid = PvmNoTask;
	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_PSTATACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, tid);
	sendmessage(mp2);
	return 0;
}
