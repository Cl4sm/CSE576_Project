ctcp_reply_backlog_change(s)
	int	s;
{
	int	i, j, delta;

	if (s <= 0)
		s = 1;
	if (server_list)
	{
		for (i = 0; i < number_of_servers; i++)
		{
			delta = s - server_list[i].ctcp_backlog_size;

			if (delta)
			{
				server_list[i].ctcp_send_size =
					(int *)new_realloc((void *)(server_list[i].ctcp_send_size), s*sizeof(int));
				for(j = server_list[i].ctcp_backlog_size; j < s; j++)
					server_list[i].ctcp_send_size[j] = 0;
				server_list[i].ctcp_backlog_size = s;
			}
		}
	}
}
