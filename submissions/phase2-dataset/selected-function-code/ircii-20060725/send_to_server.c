void
send_to_server(char *format, ...)
{
	static	int	in_send_to_server = 0;
	u_char	lbuf[BIG_BUFFER_SIZE];	/* make this buffer *much*
					 * bigger than needed */
	u_char	*buf = lbuf;
	int	des;
	size_t	len;
	int	server = from_server;
	va_list vlist;

	va_start(vlist, format);

	if (in_send_to_server)
		return;
	bzero(lbuf, sizeof(lbuf));
	in_send_to_server = 1;
	if (server == -1)
		server = primary_server;
	if (server != -1 && ((des = server_list[server].write) != -1) &&
	    (server_list[server].flags & LOGGED_IN) )
	{
		/* save space for the packet length */
		if (get_server_version(server) == ServerICB)
			buf++;
		server_list[server].sent = 1;
		vsnprintf(CP(buf), sizeof lbuf, format, vlist);
		va_end(vlist);
		len = my_strlen(buf);
		if (len > (IRCD_BUFFER_SIZE - 2))
			lbuf[IRCD_BUFFER_SIZE - 2] = '\0';
		/*
		 * for ICB, we send a final nul, and for IRC, we have
		 * a final newline.
		 */
		len++;
		if (do_hook(RAW_SEND_LIST, "%s", lbuf))
		{
			if (get_server_version(server) == ServerICB)
			{
				/*
				 * we depend on our caller to split things
				 * up for the ICB server
				 */
				if (len > 254)
					len = 254;
				lbuf[len] = 0;
				lbuf[0] = (u_char)len;
				lbuf[++len] = 0;
			}
			else
				my_strmcat(buf, "\n", IRCD_BUFFER_SIZE);

			send(des, CP(lbuf), len, 0);
		}
	}
	else if (!in_redirect && !connected_to_server)
		say("You are not connected to a server, use /SERVER to connect.");
	in_send_to_server = 0;
}
