static	void
describe(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*target;

	target = next_arg(args, &args);
	if (target && args && *args)
	{
		int	old, from_level;
#ifdef LYNX_STUFF
		u_char	*result;
#endif /* LYNX_STUFF */
		u_char	*message;

#ifdef LYNX_STUFF
		if (result = prepare_action(args))
			message = result;
		else
#endif /* LYNX_STUFF */
			message = args;

		old = set_lastlog_msg_level(LOG_ACTION);
		save_message_from();
		from_level = message_from_level(LOG_ACTION);
		send_action(target, message);
		if (do_hook(SEND_ACTION_LIST, "%s %s", target, message))
			put_it("* -> %s: %s %s", target,
				get_server_nickname(from_server), message);
		set_lastlog_msg_level(old);
		restore_message_from();

#ifdef LYNX_STUFF
		if (result)
			new_free(&result);
#endif /* LYNX_STUFF */
	}
	else
		say("Usage: /DESCRIBE <target> <action description>");
}
