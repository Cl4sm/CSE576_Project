dcc_close(args)
	u_char	*args;
{
	DCC_list	*Client;
	unsigned	flags;
	u_char	*Type;
	u_char	*user;
	u_char	*description;
	int	CType;
	u_char	*cmd = NULL;
	int	lastlog_level;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	if (!(Type = next_arg(args, &args)) || !(user=next_arg(args, &args)))
	{
		say("you must specify a type and nick for DCC CLOSE");
		goto out;
	}
	description = next_arg(args, &args);
	malloc_strcpy(&cmd, Type);
	upper(cmd);
	for (CType = 0; dcc_types[CType] != NULL; CType++)
		if (!my_strcmp(cmd, dcc_types[CType]))
			break;
	if (!dcc_types[CType])
		say("Unknown DCC type: %s", Type);
	else if ((Client = dcc_searchlist(description, user, CType, 0, description)))
	{
		flags = Client->flags;
		if (flags & DCC_DELETE)
			goto out;
		if ((flags & DCC_WAIT) || (flags & DCC_ACTIVE))
		{
			new_close(Client->read);
			if (Client->file)
				new_close(Client->file);
		}
		say("DCC %s:%s to %s closed", Type,
			description ? description : (u_char *) "<any>", user);
		dcc_erase(Client);
	}
	else
		say("No DCC %s:%s to %s found", Type,
			description ? description : (u_char *) "<any>", user);
	new_free(&cmd);
out:
	(void) set_lastlog_msg_level(lastlog_level);
}
