int
dm_execack(hp, mp)
	struct hostd *hp;
	struct pmsg *mp;
{
	struct waitc *wp;
	struct waitc_spawn *wxp;
	int rcnt;				/* num of tids+errors returned */
	int tid;
	int v;
	int err = 0;
	int i;

	if (!(wp = wait_get(hp, mp, WT_SPAWN)))
		return 0;

	wxp = (struct waitc_spawn*)wp->wa_spec;
	if (upkint(mp, &rcnt))
		goto bad;
	v = wxp->w_veclen;

	/*
	* unpack tids and place in result vector where hostpart is now
	*/

/*
	pvmlogprintf("dm_execack() hp %x vec len %d, repl len %d\n",
			hp->hd_hostpart, v, rcnt);
*/
	i = 0;
	while (rcnt-- > 0) {
		if (upkint(mp, &tid))
			goto bad;
		if (tid < 0)
			err++;
		while (i < v && wxp->w_vec[i] != hp->hd_hostpart)
			i++;
		if (i == v) {
			pvmlogerror("dm_execack() tids don't fit result vector?\n");
			wait_delete(wp);
			return 0;
		}
		wxp->w_vec[i++] = tid;
	}

	if (err)
		ht_delete(wxp->w_ht, hp);

	/*
	* if everyone has checked in, either restart the failed ones
	* or reply to the caller.
	*/

	if (wp->wa_peer == wp)
		assign_tasks(wp);
	wait_delete(wp);
	return 0;

bad:
	pvmlogprintf("dm_execack() from 0x%x bad msg format\n", mp->m_src);
	wait_delete(wp);
	return 0;
}
