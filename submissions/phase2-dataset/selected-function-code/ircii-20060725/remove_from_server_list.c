remove_from_server_list(i)
	int	i;
{
	int	old_server = from_server,
		flag = 1;
	Window	*tmp;

	from_server = i;
	clean_whois_queue();
	from_server = old_server;

	close_server(i, (u_char *) 0);

	if (server_list[i].name)
		new_free(&server_list[i].name);
	if (server_list[i].itsname)
		new_free(&server_list[i].itsname);
	if (server_list[i].password)
		new_free(&server_list[i].password);
	if (server_list[i].away)
		new_free(&server_list[i].away);
	if (server_list[i].version_string)
		new_free(&server_list[i].version_string);
	if (server_list[i].nickname)
		new_free(&server_list[i].nickname);
	if (server_list[i].group)
		new_free(&server_list[i].group);
	if (server_list[i].icbmode)
		new_free(&server_list[i].icbmode);
	if (server_list[i].whois_stuff.nick)
		new_free(&server_list[i].whois_stuff.nick);
	if (server_list[i].whois_stuff.user)
		new_free(&server_list[i].whois_stuff.user);
	if (server_list[i].whois_stuff.host)
		new_free(&server_list[i].whois_stuff.host);
	if (server_list[i].whois_stuff.channel)
		new_free(&server_list[i].whois_stuff.channel);
	if (server_list[i].whois_stuff.channels)
		new_free(&server_list[i].whois_stuff.channels);
	if (server_list[i].whois_stuff.name)
		new_free(&server_list[i].whois_stuff.name);
	if (server_list[i].whois_stuff.server)
		new_free(&server_list[i].whois_stuff.server);
	if (server_list[i].whois_stuff.server_stuff)
		new_free(&server_list[i].whois_stuff.server_stuff);
	if (server_list[i].ctcp_send_size)
		new_free(&server_list[i].ctcp_send_size);
	if (server_list[i].res0)
		freeaddrinfo(server_list[i].res0);

	/* update all the structs with server in them */
	channel_server_delete(i);	/* fix `higher' servers */
	clear_channel_list(i);
#ifndef _Windows
	exec_server_delete(i);
#endif /* _Windows */
	if (i < primary_server)
		--primary_server;
	if (i < from_server)
		--from_server;
	while ((tmp = traverse_all_windows(&flag)) != NULL)
		if (tmp->server > i && tmp->server > 0)
			tmp->server--;

	bcopy((char *) &server_list[i + 1], (char *) &server_list[i], (number_of_servers - i - 1) * sizeof(Server));
	server_list = (Server *) new_realloc(UP(server_list), --number_of_servers * sizeof(Server));

	if (from_server >= number_of_servers)
		from_server = -1;
}
