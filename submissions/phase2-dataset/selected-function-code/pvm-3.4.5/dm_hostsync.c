int
dm_hostsync(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct pmsg *mp2;
	struct timeval now;

	mp = mp;
	hp = hp;

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_HOSTSYNCACK;
	mp2->m_wid = mp->m_wid;
	gettimeofday(&now, (struct timezone *)0);
	pkint(mp2, (int)now.tv_sec);
	pkint(mp2, (int)now.tv_usec);
	sendmessage(mp2);
	return 0;
}
