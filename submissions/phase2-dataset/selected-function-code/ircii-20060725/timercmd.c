timercmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*waittime, *flag;
	struct	timeval timertime;
	long	waitsec, waitusec;
	TimerList **slot,
		  *ntimer;
	int	want = -1,
		refnum;

	if (*args == '-')
	{
		size_t	len;

		flag = next_arg(args, &args);
		len = my_strlen(flag);
		upper(flag);

		/* first check for the -DELETE flag */

		if (!my_strncmp(flag, "-DELETE", len))
		{
			u_char	*ptr;
			int	ref;
			TimerList	*tmp,
					*prev;

			if (current_exec_timer != -1)
			{
				say("You may not remove a TIMER from itself");
				return;
			}
			if (!(ptr = next_arg(args, &args)))
			{
				say("%s: Need a timer reference number for -DELETE", command);
				return;
			}
			ref = my_atoi(ptr);
			for (prev = tmp = PendingTimers; tmp; prev = tmp,
					tmp = tmp->next)
			{
				if (tmp->ref == ref)
				{
					if (tmp == prev)
						PendingTimers =
							PendingTimers->next;
					else
						prev->next = tmp->next;
					new_free(&tmp->command);
					new_free(&tmp);
					return;
				}
			}
			say("%s: Can't delete %d, no such refnum",
				command, ref);
			return;
		}
		else if (!my_strncmp(flag, "-REFNUM", len))
		{
			u_char	*ptr;

			ptr = next_arg(args, &args);
			want = my_atoi(ptr);
			if (want < 0)
			{
				say("%s: Illegal refnum %d", command, want);
				return;
			}
		}
		else
		{
			say("%s: %s no such flag", command, flag);
			return;
		}
	}

	/* else check to see if we have no args -> list */

	if (!(waittime = next_arg(args, &args)))
	{
		show_timer(command);
		return;
	}

	/* must be something to add */

	if ((refnum = create_timer_ref(want)) == -1)
	{
		say("%s: Refnum %d already exists", command, want);
		return;
	}
	
	waitusec = waitsec = 0;
	while (isdigit(*waittime))
		waitsec = waitsec*10 + ((*waittime++) - '0');
	if (*waittime == '.')
	{
		long decimalmul = 100000;
		for(; isdigit(*++waittime); decimalmul /= 10)
			waitusec += (*waittime - '0') * decimalmul;
	}
	
	gettimeofday(&timertime, NULL);	
	timertime.tv_sec += waitsec;
	timertime.tv_usec+= waitusec;
	timertime.tv_sec += (timertime.tv_usec/1000000);
	timertime.tv_usec %= 1000000;
	
	ntimer = (TimerList *) new_malloc(sizeof(TimerList));
	ntimer->in_on_who = in_on_who;
	ntimer->time = timertime.tv_sec;
	ntimer->microseconds = timertime.tv_usec;
	ntimer->server = from_server;
	ntimer->ref = refnum;
	ntimer->command = NULL;
	malloc_strcpy(&ntimer->command, args);

	/* we've created it, now put it in order */

	for (slot = &PendingTimers; *slot; slot = &(*slot)->next)
	{
		if ((*slot)->time > ntimer->time ||
		    ((*slot)->time == ntimer->time &&
		     (*slot)->microseconds > ntimer->microseconds))
			break;
	}
	ntimer->next = *slot;
	*slot = ntimer;
}
