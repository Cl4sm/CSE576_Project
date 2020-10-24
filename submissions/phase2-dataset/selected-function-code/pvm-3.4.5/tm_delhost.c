int
tm_delhost(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int count;
	char *buf;
	struct waitc *wp;

	/* sanity check the message */

	if (upkint(mp, &count))
		goto bad;
	if (count < 1 || count > (tidhmask >> (ffs(tidhmask) - 1)))
		goto bad;
	while (count-- > 0)
		if (upkstralloc(mp, &buf))
			goto bad;
		else
			PVM_FREE(buf);

	/* make a wait channel for the task */

	wp = wait_new(WT_DELHOST);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hosts->ht_hosts[hosts->ht_master]->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	/* forward message to master pvmd */

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_DELHOST;
	mp->m_wid = wp->wa_wid;
	sendmessage(mp);
	return 0;

bad:
	pvmlogprintf("tm_delhost() from t%x bad msg format\n", mp->m_src);
	return 0;
}
