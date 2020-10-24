set_history_size(size)
	int	size;
{
	int	i,
		cnt;
	History *ptr;

	if (size < hist_size)
	{
		cnt = hist_size - size;
		for (i = 0; i < cnt; i++)
		{
			ptr = command_history_tail;
			command_history_tail = ptr->prev;
			new_free(&(ptr->stuff));
			new_free(&ptr);
		}
		if (command_history_tail == (History *) 0)
			command_history_head = (History *) 0;
		else
			command_history_tail->next = (History *) 0;
		hist_size = size;
	}
}
