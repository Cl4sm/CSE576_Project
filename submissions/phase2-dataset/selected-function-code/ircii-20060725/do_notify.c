void
do_notify()
{
	static	int	location = 0;
	int	count,
		c2,
		old_server = from_server;
	u_char	buf[BIG_BUFFER_SIZE];
	NotifyList	*tmp;

	/* ICB doesn't support /notify */
	if (get_server_version(primary_server) == ServerICB)
		return;
	from_server = primary_server;
	*buf = '\0';
	for (tmp = notify_list, c2 = count = 0; tmp; tmp = tmp->next, count++)
	{
		if (count >= location && count < location + 40)
		{
			c2++;
			my_strmcat(buf, " ", sizeof buf);
			my_strmcat(buf, tmp->nick, sizeof buf);
		}
	}
	if (c2)
		add_ison_to_whois(buf, ison_notify);
	if ((location += 40) > count)
		location = 0;
	from_server = old_server;
}
