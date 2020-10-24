	struct htab *htp2;		/* new host table */
	struct htab *htp1;		/* old host table */
{
	struct pmsg *mp;
	struct htab *htp;
	struct waitc *wp, *wp2;
	int hh;

	mp = 0;
	for (wp = waitlist->wa_link; wp != waitlist; wp = wp2) {
		wp2 = wp->wa_link;
		if (wp->wa_kind == WT_HOSTA) {
			if (!mp) {
				mp = mesg_new(0);
				htp = ht_diff(htp2, htp1);
				pkint(mp, htp->ht_cnt);
				for (hh = htp->ht_last; hh > 0; hh--)
					if (htp->ht_hosts[hh])
						pkint(mp, htp->ht_hosts[hh]->hd_hostpart);
				ht_free(htp);
			}
			mp->m_ref++;
			mp->m_dst = wp->wa_mesg->m_dst;
			mp->m_ctx = wp->wa_mesg->m_ctx;
			mp->m_tag = wp->wa_mesg->m_tag;
			sendmessage(mp);
			if (wp->wa_count != -1 && --wp->wa_count < 1)
				wait_delete(wp);
		}
	}
	if (mp)
		pmsg_unref(mp);
	return 0;
}
