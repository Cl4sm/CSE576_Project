send_topic(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg;
	u_char	*arg2;

	if (get_server_version(from_server) == ServerICB)
	{
		icb_put_topic(args);
		return;
	}
	else
	if (!(arg = next_arg(args, &args)) || (my_strcmp(arg, "*") == 0))
		arg = get_channel_by_refnum(0);

	if (!arg)
	{
		say("You aren't on a channel in this window");
		return;
	}
	if (is_channel(arg))
	{
		if ((arg2 = next_arg(args, &args)) != NULL)
			send_to_server("%s %s :%s %s", command, arg,
					arg2, args);
		else
			send_to_server("%s %s", command, arg);
	}
	else
	if (get_channel_by_refnum(0))
		send_to_server("%s %s :%s", command, get_channel_by_refnum(0), subargs);
	else
		say("You aren't on a channel in this window");
}
