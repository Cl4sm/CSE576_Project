clean_up_processes()
{
	int	i;

	if (process_list_size)
	{
		say("Cleaning up left over processes....");
		for (i = 0; i < process_list_size; i++)
		{
			if (process_list[i])
				kill_process(i, SIGTERM);
		}
		sleep(2);	/* Give them a chance for a graceful exit */
		for (i = 0; i < process_list_size; i++)
		{
			if (process_list[i])
				kill_process(i, SIGKILL);
		}
	}
}
