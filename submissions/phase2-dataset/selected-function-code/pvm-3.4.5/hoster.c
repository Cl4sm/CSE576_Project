int
hoster(mp)
	struct pmsg *mp;
{
	struct pmsg *mp2;
	int num;
	int i;
	struct hst **hostlist;
	struct hst *hp;
	char *p;

	/*
	* unpack the startup message
	*/

	upkint(mp, &num);
	if (pvmdebmask & PDMSTARTUP) {
		pvmlogprintf("hoster() %d to start\n", num);
	}
	if (num > 0) {
		hostlist = TALLOC(num, struct hst *, "hsts");
		for (i = 0; i < num; i++) {
			hp = TALLOC(1, struct hst, "hst");
			hostlist[i] = hp;
			hp->h_flag = 0;
			hp->h_result = 0;
			if (upkint(mp, &hp->h_tid)
			|| upkstralloc(mp, &hp->h_sopts)
			|| upkstralloc(mp, &hp->h_login)
			|| upkstralloc(mp, &hp->h_cmd)
			|| upkstralloc(mp, &hp->h_wincmd)
			|| upkstralloc(mp, &hp->h_vmid)) {
				pvmlogerror("hoster() bad message format\n");
				pvmbailout(0);
			}
			/* Check for (possible) alternate WIN32 pvmd cmd */
			if (!strcmp(hp->h_wincmd,"")) {
				PVM_FREE(hp->h_wincmd);
				hp->h_wincmd = 0;
			}
			/* Check for (optional) virtual machine ID */
			if (!strcmp(hp->h_vmid,"")) {
				PVM_FREE(hp->h_vmid);
				hp->h_vmid = 0;
			}
			if (pvmdebmask & PDMSTARTUP) {
				pvmlogprintf("%d. t%x %s so=\"%s\"\n", i,
						hp->h_tid,
						hp->h_login,
						hp->h_sopts);
			}
			if (p = CINDEX(hp->h_login, '@')) {
				hp->h_name = STRALLOC(p + 1);
				*p = 0;
				p = STRALLOC(hp->h_login);
				PVM_FREE(hp->h_login);
				hp->h_login = p;

			} else {
				hp->h_name = hp->h_login;
				hp->h_login = 0;
			}
			if (!strcmp(hp->h_sopts, "pw"))
				hp->h_flag |= HST_PASSWORD;
			if (!strcmp(hp->h_sopts, "ms"))
				hp->h_flag |= HST_MANUAL;
		}
	}

	/*
	* do it
	*/

	pl_startup(num, hostlist);

	/*
	* send results back to pvmd
	*/

	mp2 = mesg_new(0);
	mp2->m_dst = mp->m_src;
	mp2->m_tag = DM_STARTACK;
	mp2->m_wid = mp->m_wid;
	pkint(mp2, num);
	for (i = 0; i < num; i++) {
		pkint(mp2, hostlist[i]->h_tid);
		pkstr(mp2, hostlist[i]->h_result
				? hostlist[i]->h_result : "PvmDSysErr");
	}
	if (pvmdebmask & PDMSTARTUP)
		pvmlogerror("hoster() pvmd' sending back host table\n");
	sendmessage(mp2);
	work();		/* no return */
	return 0;	/* not reached */
}
