tm_task(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int where;
	struct pmsg *mp2;
	struct waitc *wp;
	struct waitc *wp2 = 0;	/* master waitc of peer group */
	int hh;
	struct hostd *hp;

	if (upkuint(mp, &where)) {
		pvmlogerror("tm_task() bad msg format\n");
		return 0;
	}

	mp = replymessage(mp);
	mp->m_dst = tp->t_tid;
	mp->m_tag = TM_TASK;

	if (where) {		/* specific host or task requested */
		if (!(hp = tidtohost(hosts, where))) {
			pkint(mp, PvmNoHost);
			sendmessage(mp);
			return 0;
		}
		pkint(mp, 0);
		wp = wait_new(WT_TASK);
		wp->wa_mesg = mp;
		wp->wa_tid = tp->t_tid;
		wp->wa_on = hp->hd_hostpart;

		mp = mesg_new(0);
		mp->m_tag = DM_TASK;
		mp->m_dst = hp->hd_hostpart | TIDPVMD;
		mp->m_wid = wp->wa_wid;
		pkint(mp, where);
		sendmessage(mp);

	} else {			/* all tasks requested */
		pkint(mp, 0);

		wp2 = wait_new(WT_TASK);
		mp->m_ref++;
		wp2->wa_mesg = mp;
		wp2->wa_tid = tp->t_tid;

		mp2 = mesg_new(0);
		mp2->m_tag = DM_TASK;
		pkint(mp2, 0);

		for (hh = hosts->ht_last; hh > 0; hh--) {
			if (!hosts->ht_hosts[hh])
				continue;

			wp = wait_new(WT_TASK);
			mp->m_ref++;
			wp->wa_mesg = mp;
			wp->wa_tid = tp->t_tid;
			wp->wa_on = hosts->ht_hosts[hh]->hd_hostpart;

			LISTPUTBEFORE(wp2, wp, wa_peer, wa_rpeer);

			mp2->m_dst = hosts->ht_hosts[hh]->hd_hostpart | TIDPVMD;
			mp2->m_wid = wp->wa_wid;
			mp2->m_ref++;
			sendmessage(mp2);
		}
		pmsg_unref(mp2);
		pmsg_unref(mp);

		/* send message if all waiters are in */

		if (wp2->wa_peer == wp2) {
			mp->m_ref++;
			sendmessage(mp);
		}
		wait_delete(wp2);
	}
	return 0;
}
