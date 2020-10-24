int
check_wait_status(wanted)
	int wanted;
{
	Process	*proc;
#ifdef BSDWAIT
	union wait status;
#else
	int	status;
#endif /* BSDWAIT */
	int	pid,
		i;

#if defined(use_wait2) || defined(MUNIX)
	if ((pid = wait2(&status, WNOHANG, 0)) > 0)
#else
# ifdef BSDWAIT
	if ((pid = wait3(&status, WNOHANG, NULL)) > 0)
# else
	if ((pid = waitpid(wanted, &status, WNOHANG)) > 0)
# endif /* BSDWAIT */
#endif /* defined(use_wait2) || defined(MUNIX) */
	{
		if (wanted != -1 && pid == wanted)
		{
			if (WIFEXITED(status))
				return WEXITSTATUS(status);
			if (WIFSTOPPED(status))
				return - (WSTOPSIG(status));
			if (WIFSIGNALED(status))
				return - (WTERMSIG(status));
		}
		errno = 0;	/* reset errno, cause wait3 changes it */
		for (i = 0; i < process_list_size; i++)
		{
			if ((proc = process_list[i]) && proc->pid == pid)
			{
				proc->exited = 1;
				proc->termsig = WTERMSIG(status);
				proc->retcode = WEXITSTATUS(status);
				if ((proc->p_stderr == -1) &&
				    (proc->p_stdout == -1))
					delete_process(i);
				return 0;
			}
		}
	}
	return -1;
}
