e_privmsg(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*nick;
	int old_from_server = -2;

	if ((nick = next_arg(args, &args)) != NULL)
	{
		if (my_stricmp(nick, UP("-server")) == 0)
		{
			u_char *server;
			int sgroup, target_server = -1;

			if ((server = next_arg(args, &args)) == NULL)
			{
bad_server:
				say("You must supply a valid server name or "
				    "group with -server");
				goto out;
			}
			if ((nick = next_arg(args, &args)) == NULL)
				goto missing_nick;
			sgroup = find_server_group(server, 0);
			if (sgroup == 0) {
				target_server = my_atoi(server);
				if (target_server < 0 || target_server >= number_of_servers)
				{
					u_char	*port = (u_char *) 0,
						*nick2 = (u_char *) 0,
						*foo = (u_char *) 0;
					int type, port_num = 0;

					parse_server_info(&server, &port, &foo,
							  &nick2, &foo, &foo, &type);
					if (port && *port)
						port_num = my_atoi(port);
					target_server = find_in_server_list(server,
								port_num, nick2);
				}
			} else
				target_server = active_server_group(sgroup);

			if (target_server == -1)
				goto bad_server;

			old_from_server = from_server;
			from_server = target_server;
		}

		if (my_strcmp(nick, ".") == 0)
		{
			if (!(nick = sent_nick))
			{
				say("You haven't sent a message to anyone yet");
				goto out;
			}
		}

		else if (my_strcmp(nick, ",") == 0)
		{
			if (!(nick = recv_nick))
			{
				say("You have not received a message from anyone yet");
				goto out;
			}
		}
		else if (!my_strcmp(nick, "*"))
			if (!(nick = get_channel_by_refnum(0)))
				nick = zero;
		send_text(nick, args, command);
	}
	else
missing_nick:
		say("You must specify a nickname or channel!");
out:
	if (old_from_server != -2)
		from_server = old_from_server;
}
