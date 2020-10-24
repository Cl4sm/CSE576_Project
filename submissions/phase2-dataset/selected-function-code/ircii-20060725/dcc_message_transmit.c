void
dcc_message_transmit(user, text, type, flag)
	u_char	*user;
	u_char	*text;
	int	type,
		flag;
{
	DCC_list	*Client;
	u_char	tmp[BIG_BUFFER_SIZE];
	u_char	nickbuf[128];
	u_char	thing = '\0';
	u_char	*host = (u_char *) 0;
	crypt_key	*key;
	u_char	*line;
	int	lastlog_level;
	int	list = 0;
	size_t	len;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	switch(type)
	{
	case DCC_CHAT:
		host = UP("chat");
		thing = '=';
		list = SEND_DCC_CHAT_LIST;
		break;
	case DCC_RAW:
		host = next_arg(text, &text);
		if (!host)
		{
			say("No host specified for DCC RAW");
			goto out1;
		}
		break;
	}
	save_message_from();
	message_from(user, LOG_DCC);
	if (!(Client = dcc_searchlist(host, user, type, 0, (u_char *) 0)) || !(Client->flags&DCC_ACTIVE))
	{
		say("No active DCC %s:%s connection for %s", dcc_types[type], host ? host : (u_char *) "<any>", user);
		goto out;
	}
#ifdef DCC_CNCT_PEND
	/*
	 * XXX - should make this buffer
	 * XXX - just for dcc chat ?  maybe raw dcc too.  hmm.
	 */
	if (Client->flags & DCC_CNCT_PEND)
	{
		say("DCC %s:%s connection to %s is still connecting...", dcc_types[type], host ? host : (u_char *) "<any>", user);
		goto out;
	}
#endif /* DCC_DCNT_PEND */
	strmcpy(tmp, text, sizeof tmp);
	if (type == DCC_CHAT) {
		nickbuf[0] = '=';
		strmcpy(nickbuf+1, user, sizeof(nickbuf) - 2);

		if ((key = is_crypted(nickbuf)) == 0 || (line = crypt_msg(tmp, key, 1)) == 0)
			line = tmp;
	}
	else
		line = tmp;
#ifdef HAVE_WRITEV
	{
		struct iovec iov[2];

		iov[0].iov_base = CP(line);
		iov[0].iov_len = len = my_strlen(line);
		iov[1].iov_base = "\n";
		iov[1].iov_len = 1;
		len++;
		(void)writev(Client->write, iov, 2);
	}
#else
	/* XXX XXX XXX THIS IS TERRIBLE! XXX XXX XXX */
#define CRYPT_BUFFER_SIZE (IRCD_BUFFER_SIZE - 50)    /* XXX XXX FROM: crypt.c XXX XXX */
	strmcat(line, "\n", (size_t)((line == tmp) ? sizeof tmp : CRYPT_BUFFER_SIZE));
	len = my_strlen(line);
	(void)send(Client->write, line, len, 0);
#endif
	Client->bytes_sent += len;
	if (flag && type != DCC_RAW) {
		if (do_hook(list, "%s %s", Client->user, text))
			put_it("=> %c%s%c %s", thing, Client->user, thing, text);
	}
out:
	restore_message_from();
out1:
	set_lastlog_msg_level(lastlog_level);
	return;
}
