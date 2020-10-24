int
addhosts(mp, rmp)
	struct pmsg *mp;	/* the request message */
	struct pmsg *rmp;	/* reply message blank */
{
	struct hostd *hp, *hp2;
	struct pmsg *mp2;
	struct waitc *wp = 0;
	struct waitc_add *wxp = 0;
	int i, j;
	int count;
	int ngood;
	int ntid;
	struct hostent *he;
	int maxhostid = (tidhmask >> ffs(tidhmask) - 1);
	int hh;
	int pid;
	int *tids;
	char *winpvmdpath;
	char *pvmdpath;
	char *vmid;
	char *buf;
	int len;

	/*
	* have to lock this for 2 reasons:
	*  1. system can't handle overlapping host table updates,
	*  2. the new host tids aren't reserved
	*/
	if (busyadding) {
/*
		pvmlogerror("addhosts() already adding new hosts\n");
*/
		pkint(rmp, PvmAlready);
		sendmessage(rmp);
		return 0;
	}

	busyadding = 1;

	/* sanity check count */

	if (upkint(mp, &count) || count < 1 || count > maxhostid) {
		pvmlogerror("addhosts() bad msg format\n");
		goto bad;
	}

	/*
	* make wait context, extract host list from message,
	*/

	wp = wait_new(WT_HOSTSTART);
	wp->wa_tid = mp->m_src;
	wp->wa_dep = mp->m_wid;
	wxp = TALLOC(1, struct waitc_add, "waix");
	wxp->w_num = count;
	wxp->w_hosts = TALLOC(count, struct hostd *, "waiv");
	BZERO((char*)wxp->w_hosts, count * sizeof(struct hostd *));
	wp->wa_spec = (void *)wxp;

	for (i = 0; i < count; i++) {
		hp = hd_new(0);
		wxp->w_hosts[i] = hp;
		if (upkstralloc(mp, &buf)) {
			pvmlogerror("addhosts() bad msg format\n");
			goto bad;
		}
		if (parsehost(buf, hp)) {
			hp->hd_err = PvmBadParam;

		} else {

		/* Set unspecified fields from hostfile if available */

			if (filehosts &&
					((hp2 = nametohost(filehosts, hp->hd_name))
					|| (hp2 = filehosts->ht_hosts[0])))
				applydefaults(hp, hp2);
		}
		PVM_FREE(buf);
	}

	/*
	* verify we have a chance to add these babies...
	* check whether our IP is "real" or just loopback
	*/

	hp = hosts->ht_hosts[hosts->ht_local];

	if ( hp->hd_sad.sin_addr.s_addr == htonl(0x7f000001) ) {

		/* damn, we're hosed.  bail on host adds with new */
		/* PvmIPLoopback error code... */

		for (i = 0; i < count; i++) {
			hp = wxp->w_hosts[i];
			if (hp->hd_err)
				continue;
			hp->hd_err = PvmIPLoopback;
		}
	}

	/*
	* lookup IP addresses  XXX we already have some of them
	*/

	ngood = 0;
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;

		if (he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name)) {
			BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
					sizeof(struct in_addr));

		} else {
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf(
						"start_slaves() can't gethostbyname: %s\n",
						hp->hd_name);
			}
			hp->hd_err = PvmNoHost;
			continue;
		}

	/* make sure it's not already configured */

		if (!(hp->hd_flag & HF_OVERLOAD)) {
			for (hh = hosts->ht_last; hh > 0; hh--)
				if ((hp2 = hosts->ht_hosts[hh])
				&& (hp2->hd_sad.sin_addr.s_addr == hp->hd_sad.sin_addr.s_addr)) {
					hp->hd_err = PvmDupHost;
					break;
				}
			if (hp->hd_err)
				continue;

	/* make sure new ones aren't duplicated */

			for (j = i; j-- > 0; )
				if (hp->hd_sad.sin_addr.s_addr
				== wxp->w_hosts[j]->hd_sad.sin_addr.s_addr) {
					hp->hd_err = PvmDupHost;
					break;
				}
			if (hp->hd_err)
				continue;
		}

		ngood++;
	}

	/*
	* assign tids  XXX these are unreserved until hosts are added...
	*/

	ntid = ngood;
	tids = TALLOC(ngood, int, "xxx");
	hostids_new(&ntid, tids);
	if (ntid < ngood) {
		pvmlogerror("addhosts() out of hostids\n");
		ngood = ntid;
	}
	for (j = i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;
		if (j < ntid)
			hp->hd_hostpart = tids[j++];
		else
			hp->hd_err = PvmOutOfRes;
	}
	PVM_FREE(tids);

/*
	if (!ngood) {
		XXX don't really need to send the message
	}
*/

	/* keep stub reply message to caller */

	wp->wa_mesg = rmp;

	/* make request message and send to hoster or pvmd' */

	mp2 = mesg_new(0);
	mp2->m_wid = wp->wa_wid;
	pkint(mp2, ngood);
	if (!(pvmdpath = getenv("PVM_DPATH")))
		pvmdpath = PVMDPATH;
	if (!(winpvmdpath = getenv("PVM_WINDPATH")))
		winpvmdpath = WINPVMDPATH;
	for (i = 0; i < count; i++) {
		hp = wxp->w_hosts[i];
		if (hp->hd_err)
			continue;
		pkint(mp2, hp->hd_hostpart);
		pkstr(mp2, hp->hd_sopts ? hp->hd_sopts : "");

		if (hp->hd_login) {
			len = strlen(hp->hd_login)
				+ strlen((hp->hd_aname ? hp->hd_aname : hp->hd_name))
				+ 2;
			buf = TALLOC( len, char, "hdl" );
			sprintf(buf, "%s@%s", hp->hd_login,
					(hp->hd_aname ? hp->hd_aname : hp->hd_name));
		}
		else
			buf = STRALLOC( (hp->hd_aname
					? hp->hd_aname : hp->hd_name) );
		pkstr(mp2, buf);
		PVM_FREE(buf);

		/* default unix dpath */
		len = strlen( (hp->hd_dpath ? hp->hd_dpath : pvmdpath) ) + 1
			+ strlen( (hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s") ) + 1
			+ 2 + 16 + 1
			+ 2 + strlen( hp->hd_name ) + 1
			+ 5 * ( 16 + 1 );
		buf = TALLOC( len, char, "hdall" );
		(void)sprintf(buf, "%s %s -d0x%x -n%s %d %s %d",
				(hp->hd_dpath ? hp->hd_dpath : pvmdpath),
				(hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s"),
				pvmdebmask,
				hp->hd_name,
				hosts->ht_master,
				inadport_hex(
					&hosts->ht_hosts[hosts->ht_master]->hd_sad),
				hosts->ht_hosts[hosts->ht_master]->hd_mtu);
		(void)sprintf(buf + strlen(buf), " %d %s",
				((hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1)),
				inadport_hex(&hp->hd_sad));
		pkstr(mp2, buf);
		PVM_FREE(buf);

		/* default WIN32 dpath - only if not set manually (a la dx=) */
		if (!(hp->hd_dpath)){
		len = strlen( winpvmdpath ) + 1
			+ strlen( (hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s") ) + 1
			+ 2 + 16 + 1
			+ 2 + strlen( hp->hd_name ) + 1
			+ 5 * ( 16 + 1 );
		buf = TALLOC( len, char, "hdallwin" );
		(void)sprintf(buf, "%s %s -d0x%x -n%s %d %s %d",
				winpvmdpath,
				(hp->hd_sopts && !strcmp(hp->hd_sopts, "ms")
					? "-S" : "-s"),
				pvmdebmask,
				hp->hd_name,
				hosts->ht_master,
				inadport_hex(
					&hosts->ht_hosts[hosts->ht_master]->hd_sad),
				hosts->ht_hosts[hosts->ht_master]->hd_mtu);
		(void)sprintf(buf + strlen(buf), " %d %s",
				((hp->hd_hostpart & tidhmask) >> (ffs(tidhmask) - 1)),
				inadport_hex(&hp->hd_sad));
		pkstr(mp2, buf);
		PVM_FREE(buf);
		}
		/* be sure to pack SOMETHING, dammit */
		else
			pkstr(mp2, "");

		/* Include VMID (If Any) */
		if (hp->hd_vmid)
			pkstr(mp2, hp->hd_vmid);
		else if (vmid = getenv("PVM_VMID"))
			pkstr(mp2, vmid);
		/* be sure to pack SOMETHING, dammit */
		else
			pkstr(mp2, "");
	}
	mp2->m_tag = SM_STHOST;

#if !defined(WIN32) && !defined(IMA_OS2)

	if (hostertid) {
		hosterwid = wp->wa_wid;
		mp2->m_dst = hostertid;
		wp->wa_on = hostertid;
		sendmessage(mp2);

	} else {
		wp->wa_on = TIDPVMD;

		if (pid = fork()) {		/* still us */
			if (pid == -1) {
	/* nack request if can't fork */
				pvmlogperror("addhosts() fork");
				goto bad;

			} else {
				pprime = pid;
				pmsg_unref(mp2);
			}

		} else {				/* pvmd' to do the startup */
			beprime();
			mesg_rewind(mp2);
			hoster(mp2);
		}
	}

#else /* WIN32 */

	if (!hostertid)	{			/* no hoster yet */
		hostertid = tid_new();	/* give him a tid so that we can send
									him the message */

		if (start_hoster(hostertid) == -1) {
			hostertid=0;			/* you did not make it buddy */
			pvmlogperror("addhosts(): could not start hoster \n");
			goto bad;
		}
	}
	hosterwid = wp->wa_wid;
	mp2->m_dst = hostertid;
	wp->wa_on = hostertid;
	sendmessage(mp2);

#endif

	return 0;

bad:
	if (wxp)
		free_waitc_add(wxp);
	if (wp) {
		wp->wa_mesg = 0;	/* shared with rmp */
		wait_delete(wp);
	}
	busyadding = 0;
	pkint(rmp, PvmDSysErr);
	sendmessage(rmp);
	return 0;
}
