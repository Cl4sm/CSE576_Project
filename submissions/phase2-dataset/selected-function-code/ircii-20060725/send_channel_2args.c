static	void
send_channel_2args(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg1 = 0,
		*comment = 0,
	        *s = get_channel_by_refnum(0);

	args = next_arg(args, &arg1);
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

	if (arg1 && *arg1)
		arg1 = next_arg(arg1, &comment);
	
	send_to_server("%s %s %s %c%s",
		       command,
		       args,
		       arg1 && *arg1 ? arg1 : (u_char *) "",
		       comment && *comment ? ':' : ' ',
		       comment && *comment ? comment : (u_char *) "");
}
