void
query(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*nick,
		*rest;

	save_message_from();
	message_from((u_char *) 0, LOG_CURRENT);
	if ((nick = next_arg(args, &rest)) != NULL)
	{
		if (my_strcmp(nick, ".") == 0)
		{
			if (!(nick = sent_nick))
			{
				say("You have not messaged anyone yet");
				goto out;
			}
		}
		else if (my_strcmp(nick, ",") == 0)
		{
			if (!(nick = recv_nick))
			{
				say("You have not received a message from \
						anyone yet");
				goto out;
			}
		}
		else if (my_strcmp(nick, "*") == 0)
			if (!(nick = get_channel_by_refnum(0)))
			{
				say("You are not on a channel");
				goto out;
			}

#ifndef _Windows
		if (*nick == '%')
		{
			if (is_process(nick) == 0)
			{
				say("Invalid processes specification");
				goto out;
			}
		}
#endif /* _Windows */
		say("Starting conversation with %s", nick);
		set_query_nick(nick);
	}
	else
	{
		if (query_nick())
		{
			say("Ending conversation with %s", query_nick());
			set_query_nick(NULL);
		}
		else
			say("You aren't querying anyone!");
	}
	update_input(UPDATE_ALL);
out:
	restore_message_from();
}
