void
do_server(rd, wd)
	fd_set	*rd, *wd;
{
	u_char	lbuf[BIG_BUFFER_SIZE];
	int	des, j;
	static	int	times = 0;
	int	old_timeout;
#ifdef NON_BLOCKING_CONNECTS
	Win_Trav stuff;
	Window *tmp;
#endif

	for (j = 0; j < number_of_servers && !break_io_processing; j++)
	{
#ifdef NON_BLOCKING_CONNECTS
		/*
		 *	deraadt@theos.com suggests that every fd awaiting connection
		 *	should be run at this point.
		 */
		if ((des = server_list[j].write) != -1 && /*FD_ISSET(des, wd) && */
		    !(server_list[j].flags & LOGGED_IN)) {
			SOCKADDR_STORAGE sa;
			socklen_t salen = sizeof sa;

			if (getpeername(server_list[j].write, (struct sockaddr *) &sa, &salen) != -1)
				login_to_server((from_server = j));
		}
#endif /* NON_BLOCKING_CONNECTS */
		if ((des = server_list[j].read) != -1 && FD_ISSET(des, rd))
		{
			int	junk;
			u_char 	*bufptr;
			u_char	*s;
			int	i = j;
			int	old_sep = -1;
			int	is_icb;
			size_t	len;

			from_server = i;
			is_icb = get_server_version(from_server) == ServerICB;
			if (is_icb)
				old_sep = dgets_set_separator('\0');

			old_timeout = dgets_timeout(1);
			s = server_list[from_server].buffer;
			bufptr = lbuf;
			if (s && *s)
			{
				len = my_strlen(s);
				my_strncpy(lbuf, s, len);
				bufptr += len;
			}
			else
				len = 0;
			if (len >= sizeof(lbuf))
				goto buffer_is_full_hack;	/* XXX? */
			junk = dgets(bufptr, (int)(sizeof(lbuf) - len), des, (u_char *) 0);
			(void) dgets_timeout(old_timeout);
			switch (junk)
			{
			case -1:
				add_to_server_buffer(from_server, lbuf);
				goto real_continue;
			case 0:
			{
#ifdef NON_BLOCKING_CONNECTS
				int	old_serv = server_list[i].close_serv;
			/* Get this here before close_server() clears it -Sol */
				int	logged_in = server_list[i].flags & LOGGED_IN;
#endif /* NON_BLOCKING_CONNECTS */

				close_server(i, empty_string);
				say("Connection closed from %s: %s", server_list[i].name,
					dgets_errno == -1 ? "Remote end closed connection" : strerror(dgets_errno));
				server_list[i].read = server_list[i].write = -1;
#ifdef NON_BLOCKING_CONNECTS
				if (!logged_in && server_list[i].res0)
				{
					say("Trying next IP address for %s...", server_list[i].name);
					if (reconnect_to_server(i, -1)) {
						say("Connection to server %s failed...", server_list[i].name);
						clean_whois_queue();
						window_check_servers();
					}
					continue;
				}

				if (!logged_in && old_serv != -1)
				{
					if (old_serv == i)	/* a hack?  you bet */
						goto a_hack;
					if (server_list[old_serv].flags & CLOSE_PENDING)
					{
						say("Connection to server %s resumed...", server_list[old_serv].name);
						server_list[i].close_serv = -1;
						server_list[old_serv].flags &= ~(CLOSE_PENDING|CLEAR_PENDING);
						server_list[old_serv].flags |= LOGGED_IN;
						server_list[old_serv].connected = 1;
						stuff.flag = 1;
						while ((tmp = window_traverse(&stuff)))
							if (tmp->server == i)
							{
								window_set_server(tmp->refnum, old_serv, WIN_ALL);
								break;
							}
					}
					window_check_servers();
					break;
				}
a_hack:
#endif /* NON_BLOCKING_CONNECTS */
				if (i == primary_server)
				{
					if (server_list[i].eof)
					{
						say("Unable to connect to server %s",
							server_list[i].name);
						if (i == number_of_servers - 1)
  						{
							clean_whois_queue();
							window_check_servers();
							if (!connected_to_server)
								say("Use /SERVER to connect to a server");
							times = 0;
						}
						else
							server_group_get_connected_next(i);
					}
					else
					{
						if (times++ > 1)
						{
							clean_whois_queue();
							window_check_servers();
							if (!connected_to_server)
								say("Use /SERVER to connect to a server");
							times = 0;
  						}
						else
							get_connected(i);
					}
				}
				else if (server_list[i].eof)
				{
					say("Connection to server %s lost.", server_list[i].name);
					clean_whois_queue();
					window_check_servers();
				}
				else
				{
					if (reconnect_to_server(i, -1)) {
						say("Connection to server %s lost.", server_list[i].name);
						clean_whois_queue();
						window_check_servers();
					}
				}
				server_list[i].eof = 1;
				break;
			}
			default:
buffer_is_full_hack:
				{
					int	old_psi = parsing_server_index;

					parsing_server_index = i;
					server_list[parsing_server_index].parse_server(lbuf);
					new_free(&server_list[i].buffer);
					parsing_server_index = old_psi;
					break;
				}
			}
real_continue:
			from_server = primary_server;
			if (is_icb && old_sep != -1)
				(void)dgets_set_separator(old_sep);
		}
	}
}
