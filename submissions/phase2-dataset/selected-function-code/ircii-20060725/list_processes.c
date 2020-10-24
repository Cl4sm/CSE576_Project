static	void
list_processes()
{
	Process	*proc;
	int	i;
	int     lastlog_level;

	lastlog_level = set_lastlog_msg_level(LOG_CRAP);
	if (process_list)
	{
		say("Process List:");
		for (i = 0; i < process_list_size; i++)
		{
			if ((proc = process_list[i]) != NULL)
			{
				if (proc->logical)
					say("\t%d (%s): %s", i,
						proc->logical,
						proc->name);
				else
					say("\t%d: %s", i,
						proc->name);
			}
		}
	}
	else
		say("No processes are running");
	set_lastlog_msg_level(lastlog_level);
}
