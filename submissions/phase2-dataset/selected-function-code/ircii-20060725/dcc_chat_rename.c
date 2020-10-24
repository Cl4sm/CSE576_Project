dcc_chat_rename(args)
	u_char	*args;
{
	DCC_list	*Client;
	u_char	*user;
	u_char	*temp;
	
	if (!(user = next_arg(args, &args)) || !(temp = next_arg(args, &args)))
	{
		say("you must specify a current DCC CHAT connection, and a new name for it");
		return;
	}
	if (dcc_searchlist(UP("chat"), temp, DCC_CHAT, 0, (u_char *) 0))
	{
		say("You already have a DCC CHAT connection with %s, unable to rename.", temp);
		return;
	}
	if ((Client = dcc_searchlist(UP("chat"), user, DCC_CHAT, 0, (u_char *) 0)))
	{
		new_free(&(Client->user));
		malloc_strcpy(&(Client->user), temp);
		say("DCC CHAT connection with %s renamed to %s", user, temp);
	}
	else
		say("No DCC CHAT connection with %s", user);
}
