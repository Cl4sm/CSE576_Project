me(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	if (args && *args)
	{
		u_char	*target;

		if ((target = get_target_by_refnum(0)) != NULL)
		{
			int	old;
#ifdef LYNX_STUFF
			u_char	*result;
#endif /* LYNX_STUFF */
			u_char	*message;

			/* handle "/ foo" */
			if (!my_strncmp(target, get_string_var(CMDCHARS_VAR), 1) &&
			    (!(target = get_channel_by_refnum(0))))
			{
				say("No target, neither channel nor query");
				return;
			}
#ifdef LYNX_STUFF
			if (result = prepare_action(args))
				message = result;
			else
#endif /* LYNX_STUFF */

				message = args;

			old = set_lastlog_msg_level(LOG_ACTION);
			save_message_from();
			message_from(target, LOG_ACTION);
			send_action(target, message);
			if (do_hook(SEND_ACTION_LIST, "%s %s", target, message))
				put_it("* %s %s",
				    get_server_nickname(from_server), message);
			set_lastlog_msg_level(old);
			restore_message_from();

#ifdef LYNX_STUFF
			if (result)
				new_free(&result);
#endif /* LYNX_STUFF */
		}
		else
			say("No target, neither channel nor query");
	}
	else
		say("Usage: /ME <action description>");
}
