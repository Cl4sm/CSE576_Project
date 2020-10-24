send_channel_nargs(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg1 = 0,
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

	send_to_server("%s %s %s",
		       command,
		       args,
		       arg1 && *arg1 ? arg1 : (u_char *) "");
}
