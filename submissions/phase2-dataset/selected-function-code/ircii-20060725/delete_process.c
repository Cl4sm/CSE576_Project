delete_process(process)
	int	process;
{
	int	flag;
	List	*cmd,
		*next;

	if (process_list)
	{
		if (process >= process_list_size)
			return (-1);
		if (process_list[process])
		{
			Process *dead;

			if (process == wait_index)
			{
				wait_index = -1;
				irc_io_loop = 0;
			}
			dead = process_list[process];
			process_list[process] = (Process *) 0;
			if (process == (process_list_size - 1))
			{
				int	i;

				for (i = process_list_size - 1;
						process_list_size;
						process_list_size--, i--)
				{
					if (process_list[i])
						break;
				}
				if (process_list_size)
					process_list = (Process **)
						new_realloc((u_char *) process_list, sizeof(Process *) * process_list_size);
				else
				{
					new_free(&process_list);
					process_list = (Process **) 0;
				}
			}
			for (next = dead->waitcmds; next;)
			{
				cmd = next;
				next = next->next;
				parse_command(cmd->name, 0, empty_string);
				new_free(&cmd->name);
				new_free(&cmd);
			}
			dead->waitcmds = (List *) 0;
			if (dead->logical)
				flag = do_hook(EXEC_EXIT_LIST, "%s %d %d",
					dead->logical, dead->termsig,
					dead->retcode);
			else
				flag = do_hook(EXEC_EXIT_LIST, "%d %d %d",
					process, dead->termsig, dead->retcode);
			if (flag)
			{
				if (get_int_var(NOTIFY_ON_TERMINATION_VAR))
				{
					if (dead->termsig)
						say("Process %d (%s) terminated with signal %s (%d)", process, dead->name, signals[dead->termsig],
							dead->termsig);
					else
						say("Process %d (%s) terminated with return code %d", process, dead->name, dead->retcode);
				}
			}
			new_free(&dead->name);
			new_free(&dead->logical);
			new_free(&dead->who);
			new_free(&dead->redirect);
			new_free(&dead);
			return (0);
		}
	}
	return (-1);
}
