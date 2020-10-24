kill_process(kill_index, sig)
	int	kill_index,
		sig;
{
	if (process_list && (kill_index < process_list_size) && process_list[kill_index])
	{
		pid_t	pgid;

		say("Sending signal %s (%d) to process %d: %s", signals[sig], sig, kill_index, process_list[kill_index]->name);
#ifdef HAVE_GETPGID
		pgid = getpgid(process_list[kill_index]->pid);
#else
# ifdef __sgi
		pgid = BSDgetpgrp(process_list[kill_index]->pid);
# else
#  ifdef HPUX
		pgid = getpgrp2(process_list[kill_index]->pid);
#  else
#   ifdef mips	/* XXX */
		pgid = getpgrp(process_list[kill_index]->pid);
#   else
#    ifdef HAVE_GETSID
		pgid = getsid(process_list[kill_index]->pid);
#    else
#     if defined(ISC) || defined(MUNIX) || defined(BROKEN_GETPGRP)
		pgid = process_list[kill_index]->pid;
#     else
		pgid = getpgrp(process_list[kill_index]->pid);
#     endif /* ISC || MUNIX || BROKEN_GETPGRP */
#    endif /* HAVE_GETSID */
#   endif /* mips */
#  endif /* HPUX */
# endif /* __sgi */
#endif /* HAVE_GETPGID */

#if !defined(HAVE_KILLPG) && !defined(killpg)
# define killpg(pg, sig) kill(-(pg), (sig))
#endif /* HAVE_KILLPG */

		if (pgid == getpid())
		{
			yell("--- kill_process got pgid of pid!!!  something is very wrong");
			return;
		}
		killpg(pgid, sig);
		kill(process_list[kill_index]->pid, sig);
	}
	else
		say("There is no process %d", kill_index);
}
