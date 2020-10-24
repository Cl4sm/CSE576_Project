void
close_server(server_index, message)
	int	server_index;
	u_char	*message;
{
	u_char	buffer[BIG_BUFFER_SIZE];
	int	i,
		min,
		max;

	if (server_index == -1)
	{
		min = 0;
		max = number_of_servers;
	}
	else
	{
		min = server_index;
		max = server_index + 1;
	}
	for (i = min; i < max; i++)
	{
		int	old_server = from_server;

		if (server_list[i].flags & CLOSE_PENDING)
			continue;
			
		if (waiting)
			irc_io_loop = 0;
		if (i == primary_server)
			clean_whois_queue();

		from_server = -1;
		mark_not_connected(i);
		from_server = old_server;

		server_list[i].operator = 0;
		server_list[i].connected = 0;
		server_list[i].buffer = (u_char *) 0;
		server_list[i].flags = SERVER_2_6_2;
		if (-1 != server_list[i].write)
		{
			if (message && *message)
			{
				snprintf(CP(buffer), sizeof buffer, "QUIT :%s\n", message);
				send(server_list[i].write, CP(buffer), my_strlen(buffer), 0);
			}
			new_close(server_list[i].write);
			if (server_list[i].write == server_list[i].read)
				server_list[i].read = -1;
			server_list[i].write = -1;
		}
		if (-1 != server_list[i].read)
		{
			new_close(server_list[i].read);
			server_list[i].read = -1;
		}
#ifndef _Windows
		if (-1 != server_list[i].pid)
		{
			kill(server_list[i].pid, SIGKILL);
			server_list[i].pid = (pid_t) -1;
		}
#endif /* _Windows */
	}
}
