static	void
alarm_switch(flag)
	int	flag;
{
	static	int	alarm_on = 0;

	if (flag)
	{
		if (!alarm_on)
		{
			setitimer(ITIMER_REAL, &clock_timer,
				(struct itimerval *) 0);
			(void) MY_SIGNAL(SIGALRM, alarmed, 0);
			alarm_on = 1;
		}
	}
	else if (alarm_on)
	{
		setitimer(ITIMER_REAL, &off_timer, (struct itimerval *) 0);
		(void) MY_SIGNAL(SIGALRM, (sigfunc *)SIG_IGN, 0);
		alarm_on = 0;
	}
}
