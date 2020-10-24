void
display_server_list()
{
	int	i;

	/* XXX */
	if (from_server >= number_of_servers)
		from_server = -1;

	if (server_list)
	{
		if (from_server != -1)
			say("Current server: %s %d",
					server_list[from_server].name,
					server_list[from_server].port);
		else
			say("Current server: <None>");
		if (primary_server != -1)
			say("Primary server: %s %d",
				server_list[primary_server].name,
				server_list[primary_server].port);
		else
			say("Primary server: <None>");
		if (client_default_icb)
			say("Using ICB connections by default");
		say("Server list:");
		for (i = 0; i < number_of_servers; i++)
		{
			u_char	*icb_msg, *group_msg, lbuf[BIG_BUFFER_SIZE];

			icb_msg = server_list[i].version == ServerICB ? (u_char *) " (ICB connection)" : empty_string;
			if (server_list[i].server_group)
			{
				snprintf(CP(lbuf), sizeof lbuf, " [group: %s]",
				    find_server_group_name(server_list[i].server_group));
				group_msg = lbuf;
			}
			else
				group_msg = empty_string;

			if (!server_list[i].nickname)
			{
				say("\t%d) %s %d%s%s%s", i,
					server_list[i].name,
					server_list[i].port,
					server_list[i].read == -1 ? UP(" (not connected)") : empty_string,
					group_msg,
					icb_msg);
			}
			else
			{
				say("\t%d) %s %d (%s%s)%s%s", i,
					server_list[i].name,
					server_list[i].port,
					(server_list[i].read == -1) ? UP("was ") : empty_string,
					server_list[i].nickname,
					group_msg,
					icb_msg);
			}
#ifdef GKM
			say("\t\tflags: %s%s%s%s%s%s%s",
				server_list[i].flags & SERVER_2_6_2 ? UP("SERVER_2_6_2 ") : empty_string,
				server_list[i].flags & USER_MODE_I ? UP("USER_MODE_I ") : empty_string,
				server_list[i].flags & USER_MODE_W ? UP("USER_MODE_W ") : empty_string,
				server_list[i].flags & USER_MODE_S ? UP("USER_MODE_S ") : empty_string,
				server_list[i].flags & CLOSE_PENDING ? UP("CLOSE_PENDING ") : empty_string,
				server_list[i].flags & CLEAR_PENDING ? UP("CLEAR_PENDING ") : empty_string,
				server_list[i].flags & LOGGED_IN ? UP("LOGGED_IN ") : empty_string );
			say("\t\tclose_serv=%d, connected=%d, read=%d, eof=%d", server_list[i].close_serv, server_list[i].connected, server_list[i].read, server_list[i].eof);
#endif /* GKM */
		}
	}
	else
		say("The server list is empty");
}
