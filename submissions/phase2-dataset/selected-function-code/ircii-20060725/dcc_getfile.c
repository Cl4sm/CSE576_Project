dcc_getfile(args)
	u_char	*args;
{
	u_char	*user;
	u_char	*filename;
	DCC_list	*Client;
	u_char	*fullname = (u_char *) 0;

#ifdef  DAEMON_UID
	if (DAEMON_UID == getuid())
	{
		say("You are not permitted to use DCC to exchange files");
		return;
	}
#endif /* DAEMON_UID */
	if (0 == (user = next_arg(args, &args)))
	{
		say("You must supply a nickname for DCC GET");
		return;
	}
	filename = next_arg(args, &args);
	if (0 == (Client = dcc_searchlist(filename, user, DCC_FILEREAD, 0, (u_char *) 0)))
	{
		if (filename)
			say("No file (%s) offered in SEND mode by %s",
					filename, user);
		else
			say("No file offered in SEND mode by %s", user);
		return;
	}
	if ((Client->flags & DCC_ACTIVE) || (Client->flags & DCC_WAIT))
	{
		if (filename)
			say("A previous DCC GET:%s to %s exists", filename, user);
		else
			say("A previous DCC GET to %s exists", user);
		return;
	}
	if (0 == (Client->flags & DCC_OFFER))
	{
		say("I'm a teapot!");
		dcc_erase(Client);
		return;
	}

	if (0 == (fullname = expand_twiddle(Client->description)))
		malloc_strcpy(&fullname, Client->description);
	Client->file = open(CP(fullname), O_BINARY | O_WRONLY | O_TRUNC | O_CREAT, 0644);
	new_free(&fullname);
	if (-1 == Client->file)
	{
		say("Unable to open %s: %s", Client->description,
				errno ? strerror(errno) : "<No Error>");
		return;
	}
	Client->flags |= DCC_TWOCLIENTS;
	Client->bytes_sent = Client->bytes_read = 0L;
	if (!dcc_open(Client))
		close(Client->file);
}
