int
netentry(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	int c = mp->m_tag;

	if (pvmdebmask & PDMMESSAGE) {
		pvmlogprintf(
				"netentry() from host %s src t%x dst t%x tag %s wid %d\n",
				hp->hd_name, mp->m_src, mp->m_dst, pvmnametag(c, (int *)0),
				mp->m_wid);
/*
		pvmhdump(mp->m_frag->fr_link->fr_dat, mp->m_frag->fr_link->fr_len,
				"netentry() ");
*/
	}

	if (c < (int)DM_FIRST || c > (int)DM_LAST) {
		pvmlogprintf("netentry() message from t%x with bogus code %s\n",
				mp->m_src, pvmnametag(c, (int *)0));
		goto bail;
	}

	c -= DM_FIRST;
	(netswitch[c])(hp, mp);

bail:
	pmsg_unref(mp);
	return 0;
}
