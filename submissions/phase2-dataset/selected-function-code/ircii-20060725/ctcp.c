static	void
ctcp(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*to,
		*tag;
	int	ctcptype;

	if ((to = next_arg(args, &args)) != NULL)
	{
		if (!my_strcmp(to, "*"))
			if ((to = get_channel_by_refnum(0)) == NULL)
				to = zero;
		if ((tag = next_arg(args, &args)) != NULL)
			upper(tag);
		else
			tag = UP("VERSION");
		if ((ctcptype = in_ctcp()) == -1)
			my_echo(NULL, UP("*** You may not use the CTCP command in an ON CTCP_REPLY!"), empty_string);
		else
		{
			if (args && *args)
				send_ctcp(ctcp_type[ctcptype], to, tag, "%s", args);
			else
				send_ctcp(ctcp_type[ctcptype], to, tag, NULL);
		}
	}
	else
		say("Request from whom?");
}
