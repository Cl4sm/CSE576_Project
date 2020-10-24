check_process_limits()
{
	int	limit;
	int	i;
	Process	*proc;

	if ((limit = get_int_var(SHELL_LIMIT_VAR)) && process_list)
	{
		for (i = 0; i < process_list_size; i++)
		{
			if ((proc = process_list[i]) != NULL)
			{
				if (proc->counter >= limit)
				{
					proc->p_stdin = exec_close(proc->p_stdin);
					proc->p_stdout = exec_close(proc->p_stdout);
					proc->p_stderr = exec_close(proc->p_stderr);
					if (proc->exited)
						delete_process(i);
				}
			}
		}
	}
}
