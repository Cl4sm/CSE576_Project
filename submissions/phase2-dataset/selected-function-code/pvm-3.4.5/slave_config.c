int
slave_config(hn, argc, argv)
	char *hn;
	int argc;
	char **argv;
{
	int lh;			/* local host index */
	int mh;			/* master host index */
	struct hostd *hp;
	int i, j;
	int ac;
	int ms = 0;		/* manual (humanoid) startup */
#ifndef WIN32
	int dof = 1;		/* fork, exit parent (default) */
#else
	int dof = 0;
#endif
	int bad = 0;
	char *p;
	char *s;

	for (i = j = ac = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {

			case 'S':
				ms = 1;
				break;

			case 'f':
				dof = 0;
				break;

			default:
				pvmlogprintf("slave_config() unknown switch: %s\n", argv[i]);
				bad++;
			}

		} else {
			argv[j++] = argv[i];
			ac++;
		}
	}
	argc = ac;

	if (bad || argc != 6) {
		pvmlogerror("slave_config: bad args\n");
		pvmbailout(0);
	}

	mh = atoi(argv[1]);
	lh = atoi(argv[4]);
	hosts = ht_new(1);
	hosts->ht_serial = 1;
	hosts->ht_master = mh;
	hosts->ht_cons = mh;
	hosts->ht_local = lh;

	hp = hd_new(mh);
	hp->hd_name = STRALLOC("?");
	hex_inadport(argv[2], &hp->hd_sad);
	hp->hd_mtu = atoi(argv[3]);
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(0);
	hp->hd_name = STRALLOC("pvmd'");
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	hex_inadport(argv[5], &hp->hd_sad);
	ht_insert(hosts, hp);
	hd_unref(hp);

	hp = hd_new(lh);
	hp->hd_name = STRALLOC(hn);
	hp->hd_arch = STRALLOC(myarchname);
	hp->hd_mtu = pvmudpmtu;
	hp->hd_dsig = pvmmydsig;
	hex_inadport(argv[5], &hp->hd_sad);
	ht_insert(hosts, hp);
	hd_unref(hp);

	if (i = mksocs()) {
		if (i == 2) {
			printf("PvmDupHost\n");
			fflush(stdout);
		}
		pvmbailout(0);
	}

	printf("ddpro<%d> arch<%s> ip<%s> mtu<%d> dsig<%d>\n",
		DDPROTOCOL,
		myarchname,
		inadport_hex(&hp->hd_sad),
		pvmudpmtu,
		pvmmydsig);
	fflush(stdout);

#ifndef WIN32

#if !defined(IMA_OS2) && !defined(CYGWIN)
	if (!ms)
		(void)read(0, (char*)&i, 1);
#endif

	if (dof) {
		if (i = fork()) {
			if (i == -1)
				pvmlogperror("slave_config() fork");
			exit(0);
		}

	/* close everything but our sockets */

		for (i = getdtablesize(); --i >= 0; )
/* XXX don't like this - hard to maintain */
			if (i != netsock && i != loclsock && i != log_fd)
				(void)close(i);
	}

	/* reopen 0, 1, 2*/

	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_WRONLY, 0);
	(void)dup2(1, 2);

#endif

	pvmsetlog(2);

	if ((p = getenv("PVM_PATH")))
		s = STRALLOC(p);
	else
		s = STRALLOC(DEFBINDIR);
	epaths = colonsep(varsub(s));
	PVM_FREE(s);

	s = STRALLOC(DEFDEBUGGER);
	debugger = varsub(s);
	PVM_FREE(s);

	if ((s = getenv("PVM_WD")))
		p = STRALLOC(s);
	else
		p = STRALLOC(pvmgethome());
	s = varsub(p);
	if (chdir(s) == -1)
		pvmlogperror(s);
	PVM_FREE(p);
	PVM_FREE(s);

	runstate = PVMDSTARTUP;

	return 0;
}
