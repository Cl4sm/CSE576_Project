pvm_start_pvmd(argc, argv, block)
	int argc;		/* number of args to pass to pvmd (>= 0) */
	char **argv;	/* args for pvmd or null */
	int block;		/* if true, don't return until add hosts are started */
{
	char *sfn;
	struct stat sb;
	int cc;
	char *fn;			/* file to exec */
	char **av;
	int pfd[2];
	int n;
	FILE *ff;
	char buf[128];
	int x;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (pvmmytid != -1
				&& TEV_DO_TRACE(TEV_START_PVMD,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_BF, TEV_DATA_SCALAR, &block, 1, 1 );
			TEV_PACK_STRING( TEV_DID_AS, TEV_DATA_ARRAY,
				argv, argc, 1 );
			TEV_FIN;
		}
	}

	if (argc < 0 || !argv)
		argc = 0;

	if ((pvm_useruid = getuid()) == -1) {
		pvmlogerror("can't getuid()\n");
		cc = PvmSysErr;
		goto bail;
	}

	if (!(sfn = pvmdsockfile())) {
		pvmlogerror("pvm_start_pvmd() pvmdsockfile() failed\n");
		cc = PvmSysErr;
		goto bail;
	}

	if (stat(sfn, &sb) != -1) {
		cc = PvmDupHost;
		goto bail;
	}

#ifdef	IMA_TITN
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, pfd) == -1)
#else
	if (pipe(pfd) == -1)
#endif
	{
		cc = PvmSysErr;
		goto bail;
	}

	fn = pvmgetpvmd();

	av = TALLOC(argc + 2, char *, "argv");
	if (argc > 0)
		BCOPY((char *)&argv[0], (char *)&av[1], argc * sizeof(char*));
	av[0] = fn;
	av[argc + 1] = 0;

	if (!fork()) {
		(void)close(pfd[0]);
	/* fork again so the pvmd is not the child - won't have to wait() for it */
		if (!fork()) {
			if (pfd[1] != 1)
				dup2(pfd[1], 1);
			for (n = getdtablesize(); n-- > 0; )
				if (n != 1)
					(void)close(n);
			(void)open("/dev/null", O_RDONLY, 0);	/* should be 0 */
			(void)open("/dev/null", O_WRONLY, 0);	/* should be 2 */
			(void)signal(SIGINT, SIG_IGN);
			(void)signal(SIGQUIT, SIG_IGN);
			(void)signal(SIGTSTP, SIG_IGN);
			execvp(av[0], av);
		}
		_exit(0);
	}
	(void)close(pfd[1]);
	(void)wait(0);
	PVM_FREE(av);

	if (!(ff = fdopen(pfd[0], "r"))) {
		cc = PvmSysErr;
		(void)close(pfd[0]);
		goto bail;
	}

	strcpy(buf, "PVMSOCK=");
	n = strlen(buf);
	if (!fgets(buf + n, sizeof(buf) - n - 1, ff)) {
		cc = PvmCantStart;
		fclose(ff);
		goto bail;
	}
	fclose(ff);
	if (strlen(buf + n) < 2) {
		cc = PvmCantStart;
		goto bail;
	}
	n = strlen(buf);
	if (buf[n - 1] == '\n')
		buf[n - 1] = 0;
	pvmputenv(STRALLOC(buf));
/*
	fprintf(stderr, "pvm_start_pvmd: %s\n", buf);
*/

	if (cc = BEATASK)
		goto bail;

	if (block) {
		struct pvmhostinfo *hip;
		int t = 1;

		pvm_config((int*)0, (int*)0, &hip);
		while ((cc = pvm_addhosts(&hip[0].hi_name, 1, (int*)0)) == PvmAlready) {
			pvmsleep(t);
			if (t < 8)
				t *= 2;
		}
		if (cc != PvmDupHost)
			goto bail;
		cc = BEATASK;
	}

bail:

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_START_PVMD,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return (cc < 0 ? lpvmerr("pvm_start_pvmd", cc) : 0);
}
