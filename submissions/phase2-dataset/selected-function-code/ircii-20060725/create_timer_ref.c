create_timer_ref(want)
	int	want;
{
	TimerList	*tmp;
	int	ref = 0;
	int	done = 0;

	if (want == -1)
		while (!done)
		{
			done = 1;
			for (tmp = PendingTimers; tmp; tmp = tmp->next)
				if (ref == tmp->ref)
				{
					ref++;
					done = 0;
					break;
				}
		}
	else
	{
		ref = want;
		for (tmp = PendingTimers; tmp; tmp = tmp->next)
			if (ref == tmp->ref)
			{
				ref = -1;
				break;
			}
	}

	return (ref);
}
