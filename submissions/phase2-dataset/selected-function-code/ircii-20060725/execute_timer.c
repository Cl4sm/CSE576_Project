extern	void
execute_timer()
{
	struct timeval current;
	TimerList *next;
	int	old_in_on_who,
		old_server = from_server;
	
	gettimeofday(&current, NULL);

	while (PendingTimers &&
	          (PendingTimers->time < current.tv_sec
	        || (PendingTimers->time == current.tv_sec
	        &&  PendingTimers->microseconds <= current.tv_usec)))
	{
		old_in_on_who = in_on_who;
		in_on_who = PendingTimers->in_on_who;
		current_exec_timer = PendingTimers->ref;
		save_message_from();
		message_from((u_char *) 0, LOG_CRAP);
		if (PendingTimers->server >= 0)
			from_server = PendingTimers->server;
		parse_command(PendingTimers->command, 0, empty_string);
		from_server = old_server;
		restore_message_from();
		current_exec_timer = -1;
		new_free(&PendingTimers->command);
		next = PendingTimers->next;
		new_free(&PendingTimers);
		PendingTimers = next;
		in_on_who = old_in_on_who;
	}
}
