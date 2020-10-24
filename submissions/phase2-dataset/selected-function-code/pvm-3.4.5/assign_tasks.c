	struct waitc *wp;		/* (any) waitc in peer group for this op */
{
	static int lasthh = -1;			/* for assigning hosts */

	struct waitc_spawn *wxp = (struct waitc_spawn*)wp->wa_spec;
	struct htab *htp;				/* set of hosts to use */
	int *vec;						/* result/status vector */
	int veclen;						/* length of vector */
	int count = 0;					/* num of tasks to be assigned */
	int nh;							/* num of hosts to assign tasks */
	int a = 0;						/* accum for finding hosts */
	int na = 0;						/* num tasks assigned to a host */
	struct waitc *wp2;
	struct hostd *hp;
	struct pmsg *mp;
	int t;
	int i;
	int tid;
	struct timeval now;

	if (!wxp)
		return 0;

	htp = wxp->w_ht;
	vec = wxp->w_vec;
	veclen = wxp->w_veclen;

	/*
	* if no hosts left, fill unassigned entries with PvmNoHost
	*/

	if (!htp->ht_cnt)
		for (t = veclen; t-- > 0; )
			if (!vec[t])
				vec[t] = PvmNoHost;

	/*
	* count tasks to be assigned, if none left reply to task
	*/

	for (t = veclen; t-- > 0; )
		if (!vec[t])
			count++;

	if (!count) {
		pkint(wp->wa_mesg, wxp->w_veclen);
		for (t = 0; t < wxp->w_veclen; t++) {
			tid = wxp->w_vec[t];
			pkint(wp->wa_mesg, tid);
			if (TIDISTASK(tid) && wxp->w_trctid > 0) {
				tev_send_spntask(
					wxp->w_trctid, wxp->w_trcctx, wxp->w_trctag,
					tid, wxp->w_ptid );
			}
			if (TIDISTASK(tid) && wxp->w_outtid > 0) {
				mp = mesg_new(0);
				mp->m_dst = wxp->w_outtid;
				mp->m_ctx = wxp->w_outctx;
				mp->m_tag = wxp->w_outtag;
				pkint(mp, tid);
				pkint(mp, TO_SPAWN);
				pkint(mp, wxp->w_ptid);
				sendmessage(mp);
			}
		}
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;

		free_wait_spawn(wxp);
		wp->wa_spec = 0;
		return 0;
	}

	/*
	* assign tasks to hosts
	*/

	nh = min(htp->ht_cnt, count);

	/* find first host to assign */

	if (lasthh == -1)
		lasthh = hosts->ht_local + 1;
	if (lasthh > htp->ht_last)
		lasthh = 0;
	while (!htp->ht_hosts[lasthh])
		if (++lasthh > htp->ht_last)
			lasthh = 1;
	hp = htp->ht_hosts[lasthh];

	for (t = 0; t < veclen && vec[t]; t++);

	while (t < veclen) {
/*
		pvmlogprintf("assign_tasks() %s <- %d\n", hp->hd_name, t);
*/

		vec[t] = hp->hd_hostpart;
		na++;

	/* when enough tasks for this host, move to next */

		if ((a += nh) >= count) {
			a -= count;

			wp2 = wait_new(WT_SPAWN);
			wp2->wa_tid = wp->wa_tid;
			wp2->wa_on = hp->hd_hostpart;
			wp2->wa_spec = wp->wa_spec;
			wp->wa_mesg->m_ref++;
			wp2->wa_mesg = wp->wa_mesg;
			LISTPUTBEFORE(wp, wp2, wa_peer, wa_rpeer);

			mp = mesg_new(0);
			pkint(mp, wxp->w_ptid);
			pkstr(mp, wxp->w_file);
			pkint(mp, wxp->w_flags);
			pkint(mp, na);
			pkint(mp, wxp->w_argc);
			for (i = 0; i < wxp->w_argc; i++)
				pkstr(mp, wxp->w_argv[i]);
			pkint(mp, wxp->w_outtid);
			pkint(mp, wxp->w_outctx);
			pkint(mp, wxp->w_outtag);
			pkint(mp, wxp->w_trctid);
			pkint(mp, wxp->w_trcctx);
			pkint(mp, wxp->w_trctag);
			pkint(mp, wxp->w_nenv);
			for (i = 0; i < wxp->w_nenv; i++)
				pkstr(mp, wxp->w_env[i]);
			pkint(mp, (t+1)-na);	/* start proc location 0..count-1 */
			pkint(mp, count);		/* how many are there in total */
			mp->m_dst = hp->hd_hostpart | TIDPVMD;
			mp->m_tag = DM_EXEC;
			mp->m_wid = wp2->wa_wid;

			do {
				if (++lasthh > htp->ht_last)
					lasthh = 1;
			} while (!htp->ht_hosts[lasthh]);

			sendmessage(mp);

			hp = htp->ht_hosts[lasthh];
			na = 0;
		}
		for (t++ ; t < veclen && vec[t]; t++);
	}
	return 0;
}
