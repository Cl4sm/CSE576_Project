startack(wp, mp)
	struct waitc *wp;		/* wait context on hoster */
	struct pmsg *mp;
{
	struct hostd *hp;
	struct pmsg *mp2;
	struct waitc *wp2;		/* seed waitc for htupd peer group */
	struct waitc *wp3;
	int count;				/* num of new hosts */
	int happy;				/* num of happy new hosts */
	struct waitc_add *wxp;
	char *av[16];			/* for reply parsing */
	int ac;
	int ver;
	int i, j;
	int t;
	int hh;
	char *buf;
	char buf2[256];

	/*
	* unpack startup results, update hosts in wait context
	*/

	wxp = (struct waitc_add *)wp->wa_spec;
	count = wxp->w_num;
	upkint(mp, &j);

	while (j-- > 0) {
		if (upkuint(mp, &t) || upkstralloc(mp, &buf)) {
			pvmlogerror("startack() bad message format\n");
			pkint(wp->wa_mesg, PvmDSysErr);
			sendmessage(wp->wa_mesg);
			wp->wa_mesg = 0;
			wait_delete(wp);
			busyadding = 0;
			return 0;
		}
		for (i = count; i-- > 0 && wxp->w_hosts[i]->hd_hostpart != t; ) ;
		if (i < 0) {
			pvmlogprintf("startack() what? some random tid %x\n", t);
			pkint(wp->wa_mesg, PvmDSysErr);
			sendmessage(wp->wa_mesg);
			wp->wa_mesg = 0;
			wait_delete(wp);
			busyadding = 0;
			PVM_FREE(buf);
			return 0;
		}
		hp = wxp->w_hosts[i];
		ac = sizeof(av)/sizeof(av[0]);
		if (crunchzap(buf, &ac, av) || ac != 5) {
			pvmlogprintf("startack() host %s expected version, got \"%s\"\n",
					hp->hd_name, buf);
			if (!(hp->hd_err = errnamecode(buf)))
				hp->hd_err = PvmCantStart;
			PVM_FREE(buf);
			continue;
		}

		ver = atoi(av[0]);
		if (ver != DDPROTOCOL) {
			pvmlogprintf(
					"slave_exec() host %s d-d protocol mismatch (%d/%d)\n",
					hp->hd_name, ver, DDPROTOCOL);
			hp->hd_err = PvmBadVersion;
			continue;
		}

		hp->hd_arch = STRALLOC(av[1]);
		hex_inadport(av[2], &hp->hd_sad);
		hp->hd_mtu = atoi(av[3]);
		hp->hd_dsig = atoi(av[4]);

		PVM_FREE(buf);
	}

	/*
	* update reply message to add-host requestor
	*/

	mp2 = wp->wa_mesg;
	pkint(mp2, count);
	pkint(mp2, 0);	/* XXX narches = 0 for now */
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err) {
			pkint(mp2, hp->hd_err);
			pkstr(mp2, "");
			pkstr(mp2, "");
			pkint(mp2, 0);
			pkint(mp2, 0);

		} else {
			pkint(mp2, hp->hd_hostpart);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}
	}

	/*
	* delete broken ones, done now if none succeeded,
	* otherwise done when host table update is complete.
	*/

	for (j = i = 0; i < count; i++)
		if (!wxp->w_hosts[i]->hd_err) {
			hp = wxp->w_hosts[i];
			wxp->w_hosts[i] = 0;
			wxp->w_hosts[j++] = hp;

		} else {
			hd_unref(wxp->w_hosts[i]);
			wxp->w_hosts[i] = 0;
		}
	count = j;

	if (count < 1) {
		busyadding = 0;
		sendmessage(wp->wa_mesg);
		wp->wa_mesg = 0;
		free_waitc_add(wxp);
		wait_delete(wp);
		return 0;
	}

	wp2 = wait_new(WT_HTUPD);
	wp2->wa_dep = wp->wa_dep;
	wp2->wa_mesg = wp->wa_mesg;
	wp->wa_mesg = 0;

	/*
	* make next host table
	*/

	newhosts = ht_new(1);
	newhosts->ht_serial = hosts->ht_serial + 1;
	newhosts->ht_master = hosts->ht_master;
	newhosts->ht_cons = hosts->ht_cons;
	newhosts->ht_local = hosts->ht_local;

	for (i = 0; i < count; i++)
		ht_insert(newhosts, wxp->w_hosts[i]);

	free_waitc_add(wxp);
	wait_delete(wp);
	wp = 0;

	runstate = PVMDHTUPD;

	/*
	* send DM_SLCONF message to each new host
	*/

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			mp2 = mesg_new(0);
			mp2->m_tag = DM_SLCONF;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			if (hp->hd_epath) {
				pkint(mp2, DM_SLCONF_EP);
				pkstr(mp2, hp->hd_epath);
			}
			if (hp->hd_bpath) {
				pkint(mp2, DM_SLCONF_BP);
				pkstr(mp2, hp->hd_bpath);
			}
			if (hp->hd_wdir) {
				pkint(mp2, DM_SLCONF_WD);
				pkstr(mp2, hp->hd_wdir);
			}
			if (pvmschedtid) {
				sprintf(buf2, "%x", pvmschedtid);
				pkint(mp2, DM_SLCONF_SCHED);
				pkstr(mp2, buf2);
			}
			if (pvmtracer.trctid || pvmtracer.outtid) {
				sprintf(buf2, "%x %d %d %x %d %d %d %d %s",
					pvmtracer.trctid, pvmtracer.trcctx,
						pvmtracer.trctag,
					pvmtracer.outtid, pvmtracer.outctx,
						pvmtracer.outtag,
					pvmtracer.trcbuf, pvmtracer.trcopt,
					pvmtracer.tmask);
				pkint(mp2, DM_SLCONF_TRACE);
				pkstr(mp2, buf2);
			}
			sendmessage(mp2);
		}

	/*
	* create host table update message containing all current hosts
	* plus new ones, send to each new host.
	*/

	mp2 = mesg_new(0);
	mp2->m_tag = DM_HTUPD;
	pkint(mp2, newhosts->ht_serial);
	pkint(mp2, newhosts->ht_master);
	pkint(mp2, newhosts->ht_cons);
	pkint(mp2, hosts->ht_cnt + newhosts->ht_cnt);
	for (hh = hosts->ht_last; hh > 0; hh--)
		if (hp = hosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}
	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			mp2->m_ref++;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			wp3 = wait_new(WT_HTUPD);
			wp3->wa_dep = wp2->wa_dep;
			wp2->wa_mesg->m_ref++;
			wp3->wa_mesg = wp2->wa_mesg;
			wp3->wa_on = hp->hd_hostpart;
			LISTPUTBEFORE(wp2, wp3, wa_peer, wa_rpeer);
			mp2->m_wid = wp3->wa_wid;
			sendmessage(mp2);
		}
	pmsg_unref(mp2);

	/*
	* create host table update message containing happy new hosts,
	* send to each old host.
	*/

	mp2 = mesg_new(0);
	mp2->m_tag = DM_HTUPD;
	pkint(mp2, newhosts->ht_serial);
	pkint(mp2, newhosts->ht_master);
	pkint(mp2, newhosts->ht_cons);
	pkint(mp2, newhosts->ht_cnt);
	for (hh = newhosts->ht_last; hh > 0; hh--)
		if (hp = newhosts->ht_hosts[hh]) {
			pkint(mp2, hh);
			pkstr(mp2, hp->hd_name);
			pkstr(mp2, hp->hd_arch);
			pkstr(mp2, inadport_hex(&hp->hd_sad));
			pkint(mp2, hp->hd_mtu);
			pkint(mp2, hp->hd_speed);
			pkint(mp2, hp->hd_dsig);
		}

	for (hh = hosts->ht_last; hh > 0; hh--)
		if (hh != hosts->ht_local && (hp = hosts->ht_hosts[hh])) {
			mp2->m_ref++;
			mp2->m_dst = hp->hd_hostpart | TIDPVMD;
			wp3 = wait_new(WT_HTUPD);
			wp3->wa_dep = wp2->wa_dep;
			wp2->wa_mesg->m_ref++;
			wp3->wa_mesg = wp2->wa_mesg;
			wp3->wa_on = hp->hd_hostpart;
			LISTPUTBEFORE(wp2, wp3, wa_peer, wa_rpeer);
			mp2->m_wid = wp3->wa_wid;
			sendmessage(mp2);
		}
	pmsg_unref(mp2);

	/*
	* update our host table
	*/

	gotnewhosts(newhosts, hosts);

	/* XXX returning to normal state right here is a hack, we should
	   XXX wait for all the htupdacks to come back but we need the
	   XXX regular message service, hostfail entry, etc. */

	for (hh = newhosts->ht_last; hh > 0; hh--)
		if ((hp = newhosts->ht_hosts[hh]) && !hp->hd_err)
			ht_insert(hosts, hp);
	hosts->ht_serial = newhosts->ht_serial;

	if (pvmdebmask & PDMHOST) {
		pvmlogerror("startack() committing to new host table:\n");
		ht_dump(hosts);
	}
	runstate = PVMDNORMAL;
	ht_free(newhosts);
	newhosts = 0;

	/* if peered waitcs on htupdack already finished, send the reply */

	if (wp2->wa_peer == wp2) {
		busyadding = 0;
		sendmessage(wp2->wa_mesg);
		wp2->wa_mesg = 0;
	}
	wait_delete(wp2);

	return 0;
}
