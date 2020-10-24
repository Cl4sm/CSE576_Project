static	void
dcc_filesend(args)
	u_char	*args;
{
	u_char	*user;
	u_char	*filename,
		*fullname;
	DCC_list *Client;
	u_char	FileBuf[BIG_BUFFER_SIZE];
	struct	stat	stat_buf;

#ifdef  DAEMON_UID
	if (DAEMON_UID == getuid())
	{
		say("You are not permitted to use DCC to exchange files");
		return;
	}
#endif /* DAEMON_UID */
	if (0 == (user = next_arg(args, &args)) ||
	    0 == (filename = next_arg(args, &args)))
	{
		say("You must supply a nickname and filename for DCC SEND");
		return;
	}
	if (IS_ABSOLUTE_PATH(filename))
	{
		my_strmcpy(FileBuf, filename, sizeof FileBuf);
	}
	else if (*filename == '~')
	{
		if (0 == (fullname = expand_twiddle(filename)))
		{
			yell("Unable to expand %s", filename);
			return;
		}
		my_strmcpy(FileBuf, fullname, sizeof FileBuf);
		new_free(&fullname);
	}
	else
	{
		getcwd(CP(FileBuf), sizeof(FileBuf));
		my_strmcat(FileBuf, "/", sizeof FileBuf);
		my_strmcat(FileBuf, filename, sizeof FileBuf);
	}
	if (0 != access(CP(FileBuf), R_OK))
	{
		yell("Cannot access %s", FileBuf);
		return;
	}
	stat(CP(FileBuf), &stat_buf);
/* some unix didn't have this ???? */
#ifdef S_IFDIR
	if (stat_buf.st_mode & S_IFDIR)
	{
		yell("Cannot send a directory");
		return;
	}
#endif /* S_IFDER */
	if (scanstr(FileBuf, UP("/etc/")))
	{
		yell("Send request rejected");
		return;
	}
	if ((int) my_strlen(FileBuf) >= 7 && 0 == my_strcmp(FileBuf + my_strlen(FileBuf) - 7, "/passwd"))
	{
		yell("Send request rejected");
		return;
	}
	filesize = stat_buf.st_size;
	Client = dcc_searchlist(FileBuf, user, DCC_FILEOFFER, 1, filename);
	if ((Client->file = open(CP(Client->description), O_RDONLY | O_BINARY)) == -1)
	{
		say("Unable to open %s: %s\n", Client->description,
			errno ? strerror(errno) : "Unknown Host");
		new_close(Client->read);
		Client->read = Client->write = (-1);
		Client->flags |= DCC_DELETE;
		return;
	}
	filesize = 0;
	if ((Client->flags & DCC_ACTIVE) || (Client->flags & DCC_WAIT))
	{
		say("A previous DCC SEND:%s to %s exists", FileBuf, user);
		return;
	}
	Client->flags |= DCC_TWOCLIENTS;
	dcc_open(Client);
}
