got_initial_version(line)
	u_char	*line;
{
	u_char	server[256],
		version[256];
	u_char	*s, c;

	/*
	 * BROKEN_SCANF crap here provided by Aiken <adrum@u.washington.edu>
	 * sometime 1993...
	 */

#ifdef BROKEN_SCANF
	if (my_strncmp(line, "*** Your host is ", 17))
		return;
	strncpy(server, &line[17], 256;

	server[79] = 0;
	if (s = my_index(server, ','))
		*s = 0;
	if (s = my_index(server, ' '))
		*s = 0;
	version[0] = 0;

	if (s = my_index(&line[17], ' ')) 
	{
		if (!my_strncmp(c, " running version ", 17))
		{
			strncpy(version, &s[17], 255);
			version[255] = 0;
		}
                else return;
	}
	else return;
#else
	if ((sscanf(CP(line), "*** Your host is %255s running version %255s",
			server, version)) != 2) {
		yell("This server has a non-standard connection message!");
		my_strcpy(version, "2.9");
		my_strcpy(server, server_list[parsing_server_index].name);
	}
	else if ((c = server[my_strlen(server) - 1]) == ',' || c == '.')
		server[my_strlen(server) - 1] = '\0';
#endif /* BROKEN_SCANF */
	server_list[parsing_server_index].attempting_to_connect = 0;
	set_server_motd(parsing_server_index, 1);
	server_is_connected(parsing_server_index, 1);
	if ((s = my_index(server, '[')) != NULL)
		*s = '\0';	/*
				 * Handles the case where the server name is
				 * different to the host name.
				 */
	if (!my_strncmp(version, "2.5", 3))
		set_server_version(parsing_server_index, Server2_5);
	else if (!my_strncmp(version, "2.6", 3))
		set_server_version(parsing_server_index, Server2_6);
	else if (!my_strncmp(version, "2.7", 3))
		set_server_version(parsing_server_index, Server2_7);
	else if (!my_strncmp(version, "2.8", 3))
		set_server_version(parsing_server_index, Server2_8);
	else if (!my_strncmp(version, "2.9", 3))
		set_server_version(parsing_server_index, Server2_9);
	else if (!my_strncmp(version, "2.10", 4))
		set_server_version(parsing_server_index, Server2_10);
	else
		set_server_version(parsing_server_index, Server2_11);
	malloc_strcpy(&server_list[parsing_server_index].version_string, version);
	set_server_itsname(parsing_server_index, server);
	reconnect_all_channels(parsing_server_index);
	reinstate_user_modes(/* parsing_server_index */); /* XXX */
	maybe_load_ircrc();
	if (server_list[parsing_server_index].away)
		send_to_server("AWAY :%s",
			server_list[parsing_server_index].away);
	update_all_status();
	do_hook(CONNECT_LIST, "%s %d", get_server_name(parsing_server_index),
		get_server_port(parsing_server_index));
}
