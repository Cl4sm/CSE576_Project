nickname_sendline(data, nick)
	u_char	*data;
	u_char	*nick;
{
	int	new_server, server;

	new_server = my_atoi(data);
	server = parsing_server_index;
	from_server = new_server;
	if (nick && *nick)
	{
		send_to_server("NICK %s", nick);
		if (new_server == primary_server)
			malloc_strcpy(&nickname, nick);
		set_server_nickname(new_server, nick);
	}
	from_server = server;
	already_doing_reset_nickname = 0;
	update_all_status();
}
