sm_sthostack(mp)
	struct pmsg *mp;
{
	struct waitc *wp;		/* wait context on pvmd' */

	if (mp->m_src != hostertid) {
		pvmlogprintf("sm_sthostack() from t%x (not the hoster)\n",
				mp->m_src);
		return 0;
	}

	if (mp->m_wid != hosterwid) {
		pvmlogprintf("sm_sthostack() from t%x bad wid %d wanted %d\n",
				mp->m_src, mp->m_wid, hosterwid);
		return 0;
	}

	if (!(wp = wait_find(hosterwid)))
		return 0;

	hosterwid = 0;

	startack(wp, mp);

	return 0;
}
