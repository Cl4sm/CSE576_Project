void
close_all_exec()
{
	int	i;
	int	tmp;

	tmp = window_display;
	window_display = 0;
	for (i = 0; i < process_list_size; i++)
		if (process_list[i])
		{
			if (process_list[i]->p_stdin)
				new_close(process_list[i]->p_stdin);
			if (process_list[i]->p_stdout)
				new_close(process_list[i]->p_stdout);
			if (process_list[i]->p_stderr)
				new_close(process_list[i]->p_stderr);
			delete_process(i);
			kill_process(i, SIGKILL);
		}
	window_display = tmp;
}
