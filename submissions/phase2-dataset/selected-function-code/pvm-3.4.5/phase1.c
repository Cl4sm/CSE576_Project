	struct slot *sp;
{
	struct hst *hp;
	char *hn;
	char *av[32];			/* for rsh args */
	int ac;
	char buf[512];
	int pid = -1;			/* pid of rsh */
	char *p;

#ifndef NOREXEC
	struct servent *se;
	static u_short execport = 0;

	if (!execport) {
		if (!(se = getservbyname("exec", "tcp"))) {
			pvmlogprintf("phase1() can't getservbyname(): %s\n", "exec");
			pvmbailout(0);
		}
		execport = se->s_port;
		endservent();
	}
#endif

	hp = sp->s_hst;
	hn = hp->h_name;
	sp->s_rfd = sp->s_wfd = sp->s_efd = -1;

	/*
	* XXX manual startup hack... this is if we can't use rexec or rsh
	*/

	if (hp->h_flag & HST_MANUAL) {
		fprintf(stderr, "*** Manual startup ***\n");
		fprintf(stderr, "Login to \"%s\" and type:\n", hn);

		if (hp->h_wincmd) {
			fprintf(stderr, "%s\n\n", hp->h_cmd);
			fprintf(stderr, "or, if that command fails, " );
			fprintf(stderr, "for WIN32 hosts try:\n");
			fprintf(stderr, "%s\n\n", hp->h_wincmd);
		}
		else
			fprintf(stderr, "%s\n", hp->h_cmd);

	/* get version */

		fprintf(stderr, "Type response: ");
		fflush(stderr);
		if (!(fgets(buf, sizeof(buf), stdin))) {
			pvmlogprintf("host %s read error\n", hn);
			goto oops;
		}
		p = buf + strlen(buf) - 1;
		if (*p == '\n')
			*p = 0;
		hp->h_result = STRALLOC(buf);

	/* send vmid, if set */

		if (hp->h_vmid) {
			fprintf(stderr, "Now Type the Virtual Machine ID on %s:\n",
					hn);
			fprintf(stderr, "%s\n", hp->h_vmid);
		}

	/* done */

		fprintf(stderr, "Thanks\n");
		fflush(stderr);
		return 0;
	}

	/*
	* XXX end manual startup hack
	*/

	if (!(hp->h_flag & HST_PASSWORD)) {		/* use rsh to start */
		int wpfd[2], rpfd[2], epfd[2];
		int i;

		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() trying rsh to %s\n", hn);
		}

	/* fork an rsh to startup the slave pvmd */

#ifdef	IMA_TITN
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, wpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, rpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, epfd) == -1) {
			pvmlogperror("phase1() socketpair");
			goto oops;
		}
#else
		if (pipe(wpfd) == -1 || pipe(rpfd) == -1 || pipe(epfd) == -1) {
			pvmlogperror("phase1() pipe");
			goto oops;
		}
#endif

		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() pipes: %d %d %d %d %d %d\n",
					wpfd[0], wpfd[1], rpfd[0], rpfd[1], epfd[0], epfd[1]);
		}

		if ((pid = fork()) == -1) {
			pvmlogperror("phase1() fork");
			pvmbailout(0);
		}
		if (!pid) {
			(void)dup2(wpfd[0], 0);
			(void)dup2(rpfd[1], 1);
			(void)dup2(epfd[1], 2);
			for (i = getdtablesize(); --i > 2; )
				(void)close(i);
			ac = 0;
			av[ac++] = pvmgetrsh();
			av[ac++] = hn;
			if (hp->h_login) {
				av[ac++] = "-l";
				av[ac++] = hp->h_login;
			}
#ifdef IMA_BEOSCYLD
			/* Chop up the command line and all its parameters into
			 * individual strings. The underlying assumption here is
			 * that PVM_RSH is defined to specify 'bpsh' instead of
			 * 'rsh/ssh'. This bit of code converts the single string
			 * av[2] from this: "pvmd3 -option1 -option2", into this:
			 * av[2] = "pvmd3", av[3] = "-option1", av[4] = "-option2").
			 * If this is not done 'bpsh' treats the original av[2] as
			 * the file name of the command to be executed, which will
			 * fail to execute.
			 */
			p = hp->h_cmd;
			do {
				av[ac++] = p;
				p = strstr(p," ");
				if (p != NULL)
					*p++ = '\0';
			} while (p != NULL);
#else
			av[ac++] = hp->h_cmd;
#endif
			av[ac++] = 0;
			if (pvmdebmask & PDMSTARTUP) {
				for (ac = 0; av[ac]; ac++)
					fprintf(stderr, "av[%d]=\"%s\" ", ac, av[ac]);
				fputc('\n', stderr);
			}
			execvp(av[0], av);
			fputs("phase1() execvp failed\n", stderr);
			fflush(stderr);
			_exit(1);
		}
		(void)close(wpfd[0]);
		(void)close(rpfd[1]);
		(void)close(epfd[1]);
		sp->s_wfd = wpfd[1];
		sp->s_rfd = rpfd[0];
		sp->s_efd = epfd[0];

	} else {		/* use rexec to start */

#ifdef NOREXEC
		pvmlogprintf("slconfg() PVM was built without rexec support, can't take passwords\n");
		goto oops;
#else
		if (pvmdebmask & PDMSTARTUP) {
			pvmlogprintf("phase1() rexec \"%s\"\n", hp->h_cmd);
		}
		if ((sp->s_wfd = sp->s_rfd = rexec(&hn, execport,
				(hp->h_login ? hp->h_login : username),
				(char*)0, hp->h_cmd, &sp->s_efd))
		== -1) {
			pvmlogprintf("phase1() rexec failed for host %s\n", hn);
			goto oops;
		}
#endif
	}

	/* send vmid, if set */
	if (sp->s_hst->h_vmid && sp->s_wfd >= 0) {
#ifdef WIN32
		win32_write_socket(sp->s_wfd, "PVM_VMID=", 9);
		win32_write_socket(sp->s_wfd,
				sp->s_hst->h_vmid, strlen(sp->s_hst->h_vmid));
		win32_write_socket(sp->s_wfd, "\n", 1);
#else
		write(sp->s_wfd, "PVM_VMID=", 9);
		write(sp->s_wfd, sp->s_hst->h_vmid, strlen(sp->s_hst->h_vmid));
		write(sp->s_wfd, "\n", 1);
#endif
	}

	return 0;

oops:
	hp->h_result = STRALLOC("PvmCantStart");
	if (sp->s_wfd != -1)
		close(sp->s_wfd);
	if (sp->s_rfd != -1)
		close(sp->s_rfd);
	if (sp->s_efd != -1)
		close(sp->s_efd);
	sp->s_wfd = sp->s_rfd = sp->s_efd = -1;
	return 1;
}
