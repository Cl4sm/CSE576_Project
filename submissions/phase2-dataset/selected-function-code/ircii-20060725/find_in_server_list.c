find_in_server_list(server, port, nick)
	u_char	*server;
	int	port;
	u_char	*nick;
{
	int	i, maybe = -1;
	size_t	len;

	len = my_strlen(server);
	for (i = 0; i < number_of_servers; i++)
	{
		if (port && server_list[i].port &&
		    port != server_list[i].port)
			continue;

		if (my_strnicmp(server, server_list[i].name, len) != 0)
			continue;

		if (nick)
		{
			if (server_list[i].nickname == NULL)
			{
				maybe = i;
				continue;
			}
			if (my_stricmp(server_list[i].nickname, nick))
				continue;
		}
		maybe = i;
		break;
	}
	return (maybe);
}
