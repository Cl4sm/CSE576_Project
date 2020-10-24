void
register_dcc_offer(user, type, description, address, port, size)
	u_char	*user;
	u_char	*type;
	u_char	*description;
	u_char	*address;
	u_char	*port;
	u_char	*size;
{
	DCC_list	*Client;
	u_char	*c, *s, *cmd = (u_char *) 0;
	unsigned	TempInt;
	int	CType;
	int	do_auto = 0;	/* used in dcc chat collisions */
	int	lastlog_level;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	if (0 != (c = my_rindex((description), '/')))
		description = c + 1;
	if ('.' == *description)
		*description = '_';
	if (size && *size)
		filesize = my_atoi(size);
	else
		filesize = 0;
	malloc_strcpy(&cmd, type);
	upper(cmd);
	if (!my_strcmp(cmd, "CHAT"))
		CType = DCC_CHAT;
#ifndef  DAEMON_UID
	else if (!my_strcmp(cmd, "SEND"))
#else
	else if (!my_strcmp(cmd, "SEND") && DAEMON_UID != getuid())
#endif /* DAEMON_UID */
		CType = DCC_FILEREAD;
	else
	{
		say("Unknown DCC %s (%s) received from %s", type, description, user);
		goto out;
	}
	Client = dcc_searchlist(description, user, CType, 1, (u_char *) 0);
	filesize = 0;
	if (Client->flags & DCC_WAIT)
	{
		new_close(Client->read);
		dcc_erase(Client);
		if (DCC_CHAT == CType)
		{
			Client = dcc_searchlist(description, user, CType, 1, (u_char *) 0);
			do_auto = 1;
		}
		else
		{
			say("DCC %s collision for %s:%s", type, user,
				description);
			send_ctcp_reply(user, UP("DCC"), "DCC %s collision occured while connecting to %s (%s)", type, nickname, description);
			goto out;
		}
	}
	if (Client->flags & DCC_ACTIVE)
	{
		say("Received DCC %s request from %s while previous session still active", type, user);
		goto out;
	}
	Client->flags |= DCC_OFFER;

	sscanf(CP(port), "%u", &TempInt);
	if (TempInt < 1024)
	{
		say("DCC %s (%s) request from %s rejected [addr = %s, port = %d]", type, description, user, address, TempInt);
		dcc_erase(Client);
		goto out;
	}

	for (s = address; *s; s++)
		if (!isdigit(*s))
			break;
	if (*s)
		malloc_strcpy(&Client->remname, address);
	else
	{
		/* This is definately an IPv4 address, convert to a.b.c.d */
		u_char	buf[20];
		u_long	TempLong;
		u_int	dots[4], i;

		sscanf(CP(address), "%lu", &TempLong);
		if (0 == TempLong)
		{
			say("DCC %s (%s) request from %s rejected [addr = %s, port = %d]", type, description, user, address, (int)TempLong);
			dcc_erase(Client);
			goto out;
		}
		for (i = 0; i < 4; i++)
		{
			dots[i] = TempLong & 0xff;
			TempLong >>= 8;
		}
		snprintf(CP(buf), sizeof buf, "%u.%u.%u.%u", dots[3], dots[2], dots[1], dots[0]);
		malloc_strcpy(&Client->remname, buf);
	}
	Client->remport = TempInt;
	if (do_auto)
	{
		say("DCC CHAT already requested by %s, connecting to [%s:%s] ...", user, Client->remname, port);
		dcc_chat(user);
	}
	else if (Client->filesize)
		say("DCC %s (%s %lu) request received from %s [%s:%s]", type, description, (u_long)Client->filesize, user, Client->remname, port);
	else
		say("DCC %s (%s) request received from %s [%s:%s]", type, description, user, Client->remname, port);
	if (beep_on_level & LOG_CTCP)
		beep_em(1);
out:
	set_lastlog_msg_level(lastlog_level);
	new_free(&cmd);
}
