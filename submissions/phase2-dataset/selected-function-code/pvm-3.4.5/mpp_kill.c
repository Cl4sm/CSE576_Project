mpp_kill(tp, signum)
	struct task *tp;
	int signum;
{

	if (TIDISNODE(tp->t_tid)) {
		if (signum == SIGTERM || signum == SIGKILL) {
			/* sp2kill(tp->t_a_out, tp->t_tid & tidnmask); */
			(void)kill(tp->t_pid, signum);
			mpp_free(tp);
			task_cleanup(tp);
			task_free(tp);
        } else {
			sprintf(pvmtxt,"mpp_kill() signal %d to node t%x ignored\n",
				signum, tp->t_tid);
			pvmlogerror(pvmtxt);
		}
	} else
		(void)kill(tp->t_pid, signum);
}
