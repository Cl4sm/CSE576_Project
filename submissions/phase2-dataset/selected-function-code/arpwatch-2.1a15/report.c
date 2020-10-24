void
report(register char *title, register u_int32_t a, register u_char *e1,
    register u_char *e2, register time_t *t1p, register time_t *t2p,
    char *interface)
{
        extern char *path_sendmail;
	register char *cp, *hn;
	register int fd, pid;
	register FILE *f;
	char tempfile[64], cpu[64], os[64];
	char *fmt = "%20s: %s\n";
	char *watcher = mailaddress;
	char *watchee = WATCHEE;
	char *sendmail = path_sendmail;
	char *unknown = "<unknown>";
	char buf[132];
	static int init = 0;

	/* No report until we're initialized */
	if (initializing)
		return;

	if (debug) {
		if (debug > 1) {
			dosyslog(LOG_NOTICE, title, a, e1, e2, interface);
			return;
		}
		f = stdout;
		(void)putc('\n', f);
	} else {
		/* Setup child reaper if we haven't already */
		if (!init) {
			(void)setsignal(SIGCHLD, reaper);
			++init;
		}
		while (cdepth >= 3) {
			syslog(LOG_ERR, "report: pausing (cdepth %d)", cdepth);
			pause();
		}

		/* Syslog this event too */
		dosyslog(LOG_NOTICE, title, a, e1, e2, interface);

		/* return if watcher is an empty string */
		if ( quiet ) 
		  return;

		/* Update child depth */
		++cdepth;

		/* Fork off child to send mail */
		pid = fork();
		if (pid) {
			/* Parent */
			if (pid < 0)
				syslog(LOG_ERR, "report: fork() 1: %m");
			return;
		}

		/* Child */
		closelog();
		(void)strcpy(tempfile, "/tmp/arpwatch.XXXXXX");
		if ((fd = mkstemp(tempfile)) < 0) {
			syslog(LOG_ERR, "mkstemp(%s) %m", tempfile);
			exit(1);
		}
		if ((f = fdopen(fd, "w+")) == NULL) {
			syslog(LOG_ERR, "child fdopen(%s): %m", tempfile);
			exit(1);
		}
		/* Cheap delete-on-close */
		if (unlink(tempfile) < 0)
			syslog(LOG_ERR, "unlink(%s): %m", tempfile);
	}

	(void)fprintf(f, "From: arpwatch (Arpwatch %s)\n", localhost());
	(void)fprintf(f, "To: %s\n", watcher);
	if (interface == NULL) interface = ""; /* shouldn't happen */
	hn = gethname(a);
	(void)fprintf(f, "Subject: %s (%s) %s\n", title, hn,
		      interface);
	if (isdigit(*hn))
		hn = unknown;
	(void)putc('\n', f);
	(void)fprintf(f, fmt, "hostname", hn);
	(void)fprintf(f, fmt, "ip address", intoa(a));
	(void)fprintf(f, fmt, "interface", interface);
	(void)fprintf(f, fmt, "ethernet address", e2str(e1));
	if ((cp = ec_find(e1)) == NULL)
		cp = unknown;
	(void)fprintf(f, fmt, "ethernet vendor", cp);
	if (hn != unknown && gethinfo(hn, cpu, sizeof(cpu), os, sizeof(os))) {
		(void)sprintf(buf, "%s %s", cpu, os);
		(void)fprintf(f, fmt, "dns cpu & os", buf);
	}
	if (e2) {
		(void)fprintf(f, fmt, "old ethernet address", e2str(e2));
		if ((cp = ec_find(e2)) == NULL)
			cp = unknown;
		(void)fprintf(f, fmt, "old ethernet vendor", cp);
	}
	if (t1p)
		(void)fprintf(f, fmt, "timestamp", fmtdate(*t1p));
	if (t2p)
		(void)fprintf(f, fmt, "previous timestamp", fmtdate(*t2p));
	if (t1p && t2p && *t1p && *t2p)
		(void)fprintf(f, fmt, "delta", fmtdelta(*t1p - *t2p));

	if (debug) {
		fflush(f);
		return;
	}

	(void)rewind(f);
	if (dup2(fileno(f), fileno(stdin)) < 0) {
		syslog(LOG_ERR, "dup2: %m");
		exit(1);
	}
	/* XXX Need to freopen()? */
	/* Always Deliver interactively (pause when child depth gets large) */
	execl(sendmail, "sendmail", "-odi", watcher, NULL);
	syslog(LOG_ERR, "execl: %s: %m", sendmail);
	exit(1);
}