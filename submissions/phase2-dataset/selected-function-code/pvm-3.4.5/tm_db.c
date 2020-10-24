int
tm_db(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	struct waitc *wp;

	wp = wait_new(WT_DB);
	wp->wa_tid = tp->t_tid;
	wp->wa_on = hosts->ht_hosts[hosts->ht_master]->hd_hostpart;
	wp->wa_mesg = replymessage(mp);

	mp->m_ref++;
	mp->m_src = pvmmytid;
	mp->m_dst = hosts->ht_hosts[hosts->ht_master]->hd_hostpart | TIDPVMD;
	mp->m_tag = DM_DB;
	mp->m_wid = wp->wa_wid;
	sendmessage(mp);
	return 0;
}
