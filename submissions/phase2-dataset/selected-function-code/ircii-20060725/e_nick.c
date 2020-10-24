e_nick(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*nick;

	if (!(nick = next_arg(args, &args)))
	{
		say("Your nickname is %s",
			get_server_nickname(get_window_server(0)));
		return;
	}
	if (get_server_version(from_server) == ServerICB)
	{
		icb_put_nick(nick);
		return;
	}
	send_to_server("NICK %s", nick);
	if (server_list[from_server].attempting_to_connect)
		set_server_nickname(get_window_server(0), nick);
	if (get_server_version(from_server) == Server2_5)
		add_to_whois_queue(nick, whois_nickname, NULL);
}
