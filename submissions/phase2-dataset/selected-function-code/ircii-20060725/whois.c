static	void
whois(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	if (get_server_version(from_server) == ServerICB)
	{
		int	display;
		u_char	*buf;
		size_t	len;

		display = window_display;
		window_display = 0;
		len = 7 + 1 + my_strlen(args) + 1;
		buf = new_malloc(len);
		snprintf(CP(buf), len, "whereis %s", args);
		icb_put_msg2(UP("server"), buf);
		new_free(&buf);
		window_display = display;
		return;
	}

	if (args && *args)
		send_to_server("%s %s", command, args);
	else /* Default to yourself  -lynx */
		send_to_server("%s %s", command, get_server_nickname(from_server));
}
