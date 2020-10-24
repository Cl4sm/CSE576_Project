static	void
version(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*host;

	if (get_server_version(from_server) != ServerICB && ((host = next_arg(args, &args)) != NULL))
	{
		send_to_server("%s %s", command, host);
	}
	else
	{ 
		say("Client: ircII %s", irc_version);
		if (get_server_version(from_server) == ServerICB)
			icb_put_version(args);
		else
			send_to_server("%s", command);
	}
}
