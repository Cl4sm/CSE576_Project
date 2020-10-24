struct waitc *
wait_get(hp, mp, kind)
	struct hostd *hp;
	struct pmsg *mp;
	int kind;
{
	struct waitc *wp;

	if (!mp->m_wid)
		return 0;
	if (!(wp = wait_find(mp->m_wid))) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d not found\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid);
		return 0;
	}
	if (hp && hp->hd_hostpart != (wp->wa_on & TIDHOST)) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d wrong host\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid);
		return 0;
	}
	if (kind && kind != wp->wa_kind) {
		pvmlogprintf("waitc_get() tag %s from t%x wid %d wrong kind: %s\n",
				pvmnametag(mp->m_tag, (int *)0), mp->m_src, mp->m_wid,
				waitkind(wp->wa_kind));
		return 0;
	}
	return wp;
}
