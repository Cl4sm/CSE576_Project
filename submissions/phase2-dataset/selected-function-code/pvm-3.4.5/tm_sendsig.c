int
tm_sendsig(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int tid;
	struct pmsg *mp2;

	if (upkuint(mp, &tid)) {
		pvmlogerror("tm_sendsig() bad msg format\n");
		return 0;
	}
	if (!TIDISTASK(tid)) {
		pvmlogprintf("tm_sendsig() bad tid %x\n", tid);
		return 0;
	}

	mp2 = replymessage(mp);
	sendmessage(mp2);

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = (tid & TIDHOST) | TIDPVMD;
	mp->m_tag = DM_SENDSIG;
	mp->m_wid = 0;
	sendmessage(mp);
	return 0;
}
