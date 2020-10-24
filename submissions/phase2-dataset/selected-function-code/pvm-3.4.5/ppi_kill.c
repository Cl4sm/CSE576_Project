ppi_kill(tp, signum)
	struct task *tp;
	int signum;
{
	if (tp->t_pid)
#ifndef WIN32
#ifdef IMA_OS2
		(void)os2_kill(tp->t_pid, signum);
#else
		(void)kill(tp->t_pid, signum);
#endif
#else 
		(void)kill(tp->t_pid,tp->t_handle,signum);
#endif
	else
		pvmlogprintf("ppi_kill() signal for t%x scrapped (pid = 0)\n",
				tp->t_tid);
	return 0;
}
