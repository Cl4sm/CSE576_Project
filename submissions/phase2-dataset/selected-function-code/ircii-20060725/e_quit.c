e_quit(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	int	max;
	int	i;
	u_char	*Reason;

	Reason = ((args && *args) ? args : (u_char *) "Leaving");
	max = number_of_servers;
	for (i = 0; i < max; i++)
		if (is_server_connected(i))
		{
			from_server = i;
			if (get_server_version(i) != ServerICB)
				send_to_server("QUIT :%s", Reason);
		}
	irc_exit();
}
