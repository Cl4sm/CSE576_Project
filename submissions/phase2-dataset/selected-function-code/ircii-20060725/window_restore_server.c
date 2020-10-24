window_restore_server(server)
	int	server;
{
	Window	*tmp;
	int	max = number_of_servers,
		i,
		flag = 1;

	for (i = 0; i < max; i++)
	{
		while ((tmp = traverse_all_windows(&flag)) != NULL)
		{
			if (tmp->server == primary_server &&
			    tmp->prev_server == server)
			{
				tmp->server = tmp->prev_server;
				realloc_channels(tmp);
			}
		}
	}
}
