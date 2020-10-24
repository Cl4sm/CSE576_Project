main(argc, argv)
	int argc;
	char **argv;
{
	int i, j;
	char *name = "";
	struct passwd *pe;
	int testmode = 0;
	struct timeval tnow;
	char buf[128];

#ifndef WIN32
	/* check for extension config options */
	check_ext_input();

	/* make sure 0, 1, 2 are in use */
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_RDONLY, 0);
	(void)open("/dev/null", O_RDONLY, 0);
#else
	/* WSAStartup has to be called before any socket command */
	/* can be executed. Why ? Ask Bill   */

 	if (WSAStartup(0x0101, &WSAData) != 0) {
	      printf("\nWSAStartup() failed\n");
	      ExitProcess(1);
 	}
	setsockopt(INVALID_SOCKET,SOL_SOCKET,SO_OPENTYPE,
			(char *)&nAlert,sizeof(int));
	osinfo=malloc(sizeof(OSVERSIONINFO));
	osinfo->dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(osinfo);
	os_is_NT= ( osinfo->dwPlatformId == VER_PLATFORM_WIN32_NT);
#endif

	{
		char *p;
		if (p = getenv("PVMDDEBUG"))
			pvmdebmask = pvmstrtoi(p);
	}

#ifndef WIN32
	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("main() can't getuid()\n");
		pvmbailout(0);
	}
#else
	username = MyGetUserName();
#endif

	pvmsetlog(3);

#ifndef WIN32
	if (pe = getpwuid(pvm_useruid))
		username = STRALLOC(pe->pw_name);
	else
		pvmlogerror("main() can't getpwuid\n");
	endpwent();
#endif

#ifdef WIN32
	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("main() can't getpid() %d \n",GetLastError());
		pvmbailout(0);
	}
#else
	if ((pvmmyupid = getpid()) == -1) {
		pvmlogerror("main() can't getpid()\n");
		pvmbailout(0);
	}
#endif

	(void)pvmgetroot();		/* fail here if we can't */

	sprintf(buf, "PVM_ARCH=%s", myarchname);
	pvmputenv(STRALLOC(buf));

	pvmmydsig = pvmgetdsig();

	ppi_config(argc, argv);

#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
	mpp_init(&argc, argv);
#endif

	for (i = j = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {

			case 'd':
				pvmdebmask = pvmstrtoi(argv[i] + 2);
				break;

			case 'n':
				name = argv[i] + 2;
				break;

			case 'S':
				argv[j++] = argv[i];
			case 's':
				slavemode = 1;
				break;

			case 't':
				testmode = 1;
				break;

#ifdef WIN32
			case 'u':
				if (os_is_NT==FALSE) {
					/* someone is on win95 ... */
					argv[i]++;argv[i]++;
					strcpy(username,argv[i]);
				}
				else
					if (strcmp(strcat("-u",username),argv[i])) {
						fprintf(stderr,
								"Provided Username(%s) does not match ",
								username);
						fprintf(stderr,"with OS account(%s).\n",
								argv[i]);
						fprintf(stderr,"Wrong rshd ?!\n");
						exit(1);
				}
				break;
#endif

			default:
				argv[j++] = argv[i];
			}

		} else {
			argv[j++] = argv[i];
		}
	}
	argc = j;

	if (pvmdebmask) {
		pvmlogprintf("version %s\n", PVM_VER);
		pvmlogprintf("ddpro %d tdpro %d\n", DDPROTOCOL, TDPROTOCOL);
		pvmlogprintf("main() debug mask is 0x%x (%s)\n",
				pvmdebmask, debug_flags(pvmdebmask));
	}

	if (!*name) {
		if (gethostname(buf, sizeof(buf)-1) == -1) {
			pvmlogerror("main() can't gethostname()\n");
			pvmbailout(0);
		}
		name = buf;
	}
	if (testmode) {
		PVM_TIMET time_temp;
		gettimeofday(&tnow, (struct timezone*)0);
		pvmlogprintf("version %s ddpro %d tdpro %d sdpro %d\n",
				PVM_VER, DDPROTOCOL, TDPROTOCOL, SDPROTOCOL);
		time_temp = (PVM_TIMET) tnow.tv_sec;
		pvmlogprintf(ctime(&time_temp));
		for (i = 0; i < argc; i++)
			pvmlogprintf("argv[%d]=\"%s\"\n", i, argv[i]);
		exit(0);
	}

	{
		char *p;
		if (p = getenv("PVM_MAX_TASKS")) {
			pvm_max_ntasks = atoi(p);
			if (pvm_max_ntasks < 0)
				pvm_max_ntasks = 0;
		}
	}

	if (slavemode)					/* slave pvmd */
		slave_config(name, argc, argv);

	else							/* master pvmd */
		master_config(name, argc, argv);

#if TTYDIS & 8
	setsid();
#endif
#if TTYDIS & 4
	setpgid(0, 0);
#endif
#if TTYDIS & 2
	setpgrp(0, 0);
#endif
#if TTYDIS & 1
	if ((i = open("/dev/tty", O_RDWR, 0)) != -1) {
		(void)ioctl(i, TIOCNOTTY, 0);
		(void)close(i);
	}
#endif

	myhostpart = hosts->ht_local << (ffs(tidhmask) - 1);
	pvmmytid = myhostpart | TIDPVMD;

	ndead = 1000;	/* XXX hum, static limit makes this easy to do */

	/* deads = TALLOC(ndead, int, "pids"); */

	deads = TALLOC(ndead, struct deaddata, "dead");
	BZERO((char*)deads, ndead * sizeof(struct deaddata));

#ifndef WIN32
	/* no signaling in win32 because no parent child relation ... :-( */

#ifndef IMA_I860 /* this signal interferes with getcube() on I860 */
#ifdef	SYSVSIGNAL
#ifndef IMA_BEOSCYLD
	(void)signal(SIGCLD, reap);
#endif
#ifdef IMA_SUNMP
	sigset(SIGCLD, reap); /* yep we go really want to catch our kids */
#endif
#else
	(void)signal(SIGCHLD, reap);
#endif	/* SYSVSIGNAL */
#endif /*IMA_I860*/

#endif

	if (signal(SIGINT, SIG_IGN) != SIG_IGN)
#ifdef WIN32
		(void)signal(SIGINT, (void*) catch);
#else
		(void)signal(SIGINT, catch);
#endif

	if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
#ifdef WIN32
	(void)signal(SIGTERM, (void *) catch);
#else
	(void)signal(SIGTERM, catch);
#endif

#ifndef WIN32
	(void)signal(SIGHUP, SIG_IGN);
	(void)signal(SIGPIPE, SIG_IGN);
#endif

#ifndef WIN32
	(void)signal(SIGFPE, evilsig);
	(void)signal(SIGILL, evilsig);
#else
	(void)signal(SIGFPE, (void *)evilsig);
	(void)signal(SIGILL, (void *)evilsig);
#endif

#ifdef	SIGBUS
	(void)signal(SIGBUS, evilsig);
#endif

#ifndef WIN32
	(void)signal(SIGSEGV, evilsig);
#else
	(void)signal(SIGSEGV, (void *)evilsig);
#endif

#ifdef	SIGSYS
	(void)signal(SIGSYS, evilsig);
#endif

#ifdef	SIGDANGER
	(void)signal(SIGDANGER, biteme);
#endif

#ifdef	STATISTICS
	reset_statistics();
#endif

	task_init();
	wait_init(myhostpart, TIDLOCAL);
	mb_init();
	ppi_init();

	opq = pk_new(0);
	opq->pk_tlink = opq->pk_trlink = opq;

	/* print local socket address on stdout in case someone cares */

	if (!slavemode) {
		printf("%s\n", getenv("PVMSOCK"));
		fflush(stdout);
	}

/* XXX hack to start slaves automatically */

	if (!slavemode && filehosts) {
		struct hostd *hp;
		int hh;
		int n = 0;

		for (hh = filehosts->ht_last; hh >= 1; hh--)
			if ((hp = filehosts->ht_hosts[hh]) && !(hp->hd_flag & HF_NOSTART))
				n++;
		if (n) {
			addmesg = mesg_new(0);
			addmesg->m_tag = DM_ADD;
			pkint(addmesg, n);
			for (hh = 1; hh <= filehosts->ht_last; hh++)
				if ((hp = filehosts->ht_hosts[hh]) && !(hp->hd_flag & HF_NOSTART))
					pkstr(addmesg, hp->hd_name);
			addmesg->m_dst = TIDPVMD;
		}
	}

	work();
	pvmbailout(0);		/* not reached */
	exit(0);
}
