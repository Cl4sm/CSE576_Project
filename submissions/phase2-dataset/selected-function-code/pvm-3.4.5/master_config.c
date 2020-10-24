master_config(hn, argc, argv)
	char *hn;			/* hostname or null */
	int argc;
	char **argv;
{
	struct hostent *he;
	struct hostd *hp;
	struct hostd *hp2;
	int i;
	char *s;

	if (argc > 2) {
		pvmlogerror("usage: pvmd3 [-ddebugmask] [-nhostname] [hostfile]\n");
		pvmbailout(0);
	}
	if (argc == 2) {
		filehosts = readhostfile(argv[1]);
	}
	else if (s = getenv("PVMHOSTFILE")) {
		filehosts = readhostfile(s);
	}
	if (pvmdebmask & PDMSTARTUP) {
		if (filehosts) {
			pvmlogerror("master_config() host file:\n");
			ht_dump(filehosts);

		} else
			pvmlogerror("master_config() null host file\n");
	}

	hosts = ht_new(1);
	hosts->ht_serial = 1;
	hosts->ht_master = 1;
	hosts->ht_cons = 1;
	hosts->ht_local = 1;

	hp = hd_new(1);
	hp->hd_name = STRALLOC(hn);
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(0);
	hp->hd_name = STRALLOC("pvmd'");
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	ht_insert(hosts, hp);
	hd_unref(hp);

	/*
	* get attributes from host file if available
	*/

	hp = hosts->ht_hosts[1];
	if (filehosts &&
			((hp2 = nametohost(filehosts, hp->hd_name))
			|| (hp2 = filehosts->ht_hosts[0]))) {
		applydefaults(hp, hp2);
	}

	if (!hp->hd_epath) {
		if ((s = getenv("PVM_PATH")))
			hp->hd_epath = STRALLOC(s);
		else
			hp->hd_epath = STRALLOC(DEFBINDIR);
	}
	epaths = colonsep(varsub(hp->hd_epath));
	if (!hp->hd_bpath)
		hp->hd_bpath = STRALLOC(DEFDEBUGGER);
	debugger = varsub(hp->hd_bpath);
	if (!hp->hd_wdir) {
		if ((s = getenv("PVM_WD")))
			hp->hd_wdir = STRALLOC(s);
		else
			hp->hd_wdir = STRALLOC(pvmgethome());
	}
	s = varsub(hp->hd_wdir);
	if (chdir(s) == -1)
		pvmlogperror(s);
	PVM_FREE(s);

	if (!(he = gethostbyname(hp->hd_aname ? hp->hd_aname : hp->hd_name))) {
		pvmlogprintf("master_config() %s: can't gethostbyname\n", hn);
		pvmbailout(0);
	}

	BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
		sizeof(struct in_addr));

	hp = hosts->ht_hosts[0];
	BCOPY(he->h_addr_list[0], (char*)&hp->hd_sad.sin_addr,
		sizeof(struct in_addr));

	if (pvmdebmask & (PDMHOST|PDMSTARTUP)) {
		pvmlogerror("master_config() host table:\n");
		ht_dump(hosts);
	}

	if (mksocs())
		pvmbailout(0);


#ifndef WIN32
	/* close everything but our sockets */
	for (i = getdtablesize(); --i > 2; )
/* XXX don't like this - hard to maintain */
		if (i != netsock && i != ppnetsock && i != loclsock && i != log_fd)
			(void)close(i);

	/* reopen 0, 1, 2*/
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_WRONLY, 0);
	(void)dup2(1, 2);
#endif

	pvmsetlog(2);

	runstate = PVMDNORMAL;
	return 0;
}
