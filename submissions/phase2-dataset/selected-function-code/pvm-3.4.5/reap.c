reap(sig)
	int sig;
{
	int pid;
	int es = 0;

#ifndef WIN32

#ifndef NOWAIT3
#if defined(RUSAGE_SELF)
	struct rusage rus;
#else
	int rus;
#endif
#endif

	sig = sig;

#ifdef	NOWAIT3
#ifdef	NOWAITPID
	if ((pid = wait(&es)) > 0)
#else
#ifdef IMA_BEOSCYLD
	if ((pid = waitpid(-1, &es, WNOHANG)) > 0)
#else
	while ((pid = waitpid(-1, &es, WNOHANG)) > 0)
#endif
#endif
#else	/*NOWAIT3*/
	while ((pid = wait3(&es, WNOHANG, &rus)) > 0)
#endif	/*NOWAIT3*/

	{
#if !defined(NOWAIT3) && defined(RUSAGE_SELF)
		deads[wdead].dd_ut = rus.ru_utime;
		deads[wdead].dd_st = rus.ru_stime;
#else
		deads[wdead].dd_ut.tv_sec = 0;
		deads[wdead].dd_ut.tv_usec = 0;
		deads[wdead].dd_st.tv_sec = 0;
		deads[wdead].dd_st.tv_usec = 0;
#endif
		deads[wdead].dd_pid = pid;
		deads[wdead].dd_es = es;
		if (++wdead >= ndead)
			wdead = 0;
	}

#ifdef	SYSVSIGNAL
#ifndef IMA_BEOSCYLD
	(void)signal(SIGCLD, reap);
#endif
#endif

#endif

}
