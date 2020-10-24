evilsig(sig)
	int sig;
{
	if (runstate == PVMDISTASK)
		exit(sig);
	(void)signal(SIGILL, SIG_DFL);
	(void)signal(SIGFPE, SIG_DFL);
#ifdef	SIGBUS
	(void)signal(SIGBUS, SIG_DFL);
#endif
	(void)signal(SIGSEGV, SIG_DFL);
#ifdef	SIGSYS
	(void)signal(SIGSYS, SIG_DFL);
#endif
	(void)signal(SIGINT, SIG_DFL);
	(void)signal(SIGTERM, SIG_DFL);
#ifndef WIN32
	(void)signal(SIGHUP, SIG_DFL);
	(void)signal(SIGPIPE, SIG_DFL);
#endif
#ifdef	SYSVSIGNAL
	(void)signal(SIGCLD, SIG_DFL);
#else
#ifndef WIN32
	(void)signal(SIGCHLD, SIG_DFL);
#endif
#endif
	pvmlogprintf("evilsig() caught signal %d\n", sig);
	i_dump(1);
/*
	abort();
*/
	pvmbailout(-sig);
}
