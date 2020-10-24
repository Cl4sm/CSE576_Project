int
tm_hoster(tp, mp)
	struct task *tp;
	struct pmsg *mp;
{
	int regme;
	struct pmsg *mp2;

	if (upkint(mp, &regme)) {
		pvmlogerror("tm_hoster() bad msg format\n");
		return 0;
	}

	mp2 = replymessage(mp);

	/* always wear 2 condoms, who knows if indices are enough... :) */
	if (hosts->ht_hosts[hosts->ht_local]->hd_hostpart
			!= hosts->ht_hosts[hosts->ht_master]->hd_hostpart) {
		pkint(mp2, PvmHostrNMstr);
		sendmessage(mp2);
		return 0;
	}

	if (regme) {
		if (hostertid) {
			pkint(mp2, PvmAlready);

		} else {
			hostertid = tp->t_tid;
			tp->t_flag |= TF_ISHOSTER;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() register t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);
		}

	} else {
		if (hostertid == tp->t_tid) {
			hostertid = 0;
			tp->t_flag &= ~TF_ISHOSTER;
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() unregister t%x \n", tp->t_tid);
			}
			pkint(mp2, 0);

		} else {
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("tm_hoster() t%x tries to unregister?\n",
						tp->t_tid);
			}
			pkint(mp2, PvmNoTask);
		}
	}

	sendmessage(mp2);
	return 0;
}
