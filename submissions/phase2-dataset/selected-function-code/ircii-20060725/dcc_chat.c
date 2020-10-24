static void
dcc_chat(args)
	u_char	*args;
{
	u_char	*user;
	DCC_list	*Client;

	if ((user = next_arg(args, &args)) == NULL)
	{
		say("You must supply a nickname for DCC CHAT");
		return;
	}
	Client = dcc_searchlist(UP("chat"), user, DCC_CHAT, 1, (u_char *) 0);
	if ((Client->flags&DCC_ACTIVE) || (Client->flags&DCC_WAIT))
	{
		say("A previous DCC CHAT to %s exists", user);
		return;
	}
	Client->flags |= DCC_TWOCLIENTS;
	dcc_open(Client);
}
