dm_htupdack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;

	if (!(wp = wait_get(hp, mp, WT_HTUPD)))
		return 0;

	/* is this is the last host checking in, send ht commit */

	if (wp->wa_peer == wp) {
		int hh;

		mp = mesg_new(0);
		mp->m_tag = DM_HTCOMMIT;

		for (hh = hosts->ht_last; hh > 0; hh--)
			if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
				mp->m_ref++;
				mp->m_dst = hp->hd_hostpart | TIDPVMD;
				sendmessage(mp);
			}
		pmsg_unref(mp);

		busyadding = 0;
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
	}
	wait_delete(wp);
	return 0;
}
