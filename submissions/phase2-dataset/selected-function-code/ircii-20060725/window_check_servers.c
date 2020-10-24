window_check_servers()
{
	Window	*tmp;
	int	flag, cnt, max, i, not_connected,
	prime = -1;

	connected_to_server = 0;
	max = number_of_servers;
	for (i = 0; i < max; i++)
	{
		not_connected = !is_server_open(i);
		flag = 1;
		cnt = 0;
		while ((tmp = traverse_all_windows(&flag)) != NULL)
		{
			if (tmp->server == i)
			{
				if (not_connected)
				{
					tmp->server = primary_server;
					if (tmp->current_channel)
						new_free(&tmp->current_channel);
				}
				else
				{
					prime = tmp->server;
					cnt++;
				}
			}
		}
		if (cnt == 0)
		{
#ifdef NON_BLOCKING_CONNECTS
			if (!(server_list[i].flags & CLOSE_PENDING))
#endif /* NON_BLOCKING_CONNECTS */
			close_server(i, empty_string);
		}
		else
			connected_to_server++;
	}
	if (!is_server_open(primary_server))
	{
		flag = 1;
		while ((tmp = traverse_all_windows(&flag)) != NULL)
			if (tmp->server == primary_server)
			{
				tmp->server = prime;
			}
		primary_server = prime;
	}
	update_all_status();
	cursor_to_input();
}
