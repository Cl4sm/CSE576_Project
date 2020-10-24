dcc_rename(args)
	u_char	*args;
{
	DCC_list	*Client;
	u_char	*user;
	u_char	*description;
	u_char	*newdesc;
	u_char	*temp;
	int	lastlog_level;

	lastlog_level = set_lastlog_msg_level(LOG_DCC);
	if ((user = next_arg(args, &args)) && my_strnicmp(user, UP("-chat"), my_strlen(user)) == 0)
	{
		dcc_chat_rename(args);
		return;
	}
	if (!user || !(temp = next_arg(args, &args)))
	{
		say("you must specify a nick and new filename for DCC RENAME");
		goto out;
	}
	if ((newdesc = next_arg(args, &args)) != NULL)
		description = temp;
	else
	{
		newdesc = temp;
		description = NULL;
	}
	if ((Client = dcc_searchlist(description, user, DCC_FILEREAD, 0, (u_char *) 0)))
	{
		if (!(Client->flags & DCC_OFFER))
		{
			say("Too late to rename that file");
			goto out;
		}
		new_free(&(Client->description));
		malloc_strcpy(&(Client->description), newdesc);
		say("File %s from %s renamed to %s",
			 description ? description : (u_char *) "<any>", user, newdesc);
	}
	else
		say("No file %s from %s found",
			description ? description : (u_char *) "<any>", user);
out:
	(void) set_lastlog_msg_level(lastlog_level);
}
