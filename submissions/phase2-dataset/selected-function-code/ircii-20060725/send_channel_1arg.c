static	void
send_channel_1arg(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*comment,
		*s = get_channel_by_refnum(0);

	args = next_arg(args, &comment);

	if (!args || !my_strcmp(args, "*"))
	{
		if (s)
			args = s;
		else
		{
			say("You aren't on a channel in this window");
			return;
		}
	}

	send_to_server("%s %s %c%s",
		       command,
		       args,
		       comment && *comment ? ':' : ' ',
		       comment && *comment ? comment : (u_char *) "");
}
