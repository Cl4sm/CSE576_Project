static	void
show_timer(command)
	u_char	*command;
{
	u_char  lbuf[BIG_BUFFER_SIZE];
	TimerList *tmp;
	struct timeval current, time_left;

	if (!PendingTimers)
	{
		say("%s: No commands pending to be executed", command);
		return;
	}

	gettimeofday(&current, NULL);
	say("Timer Seconds      Command");
	for (tmp = PendingTimers; tmp; tmp = tmp->next)
	{
		time_left.tv_sec = tmp->time;
		time_left.tv_usec = tmp->microseconds;
		time_left.tv_sec -= current.tv_sec;

		if (time_left.tv_usec >= current.tv_usec)
			time_left.tv_usec -= current.tv_usec;
		else
		{
			time_left.tv_usec = time_left.tv_usec -
			    current.tv_usec + 1000000;
			time_left.tv_sec--;
		}

		snprintf(CP(lbuf), sizeof(lbuf), "%ld.%06d",
		    (long)time_left.tv_sec, (int)time_left.tv_usec);
		say("%-5d %-12s %s", tmp->ref, lbuf, tmp->command);
	}
}
