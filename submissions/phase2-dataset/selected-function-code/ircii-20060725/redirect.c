redirect(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*to;

	if ((to = next_arg(args, &args)) != NULL)
	{
		if (!my_strcmp(to, "*"))
			if (!(to = get_channel_by_refnum(0)))
			{
				say("Must be on a channel to redirect to '*'");
				return;
			}
		if (!my_stricmp(to, get_server_nickname(from_server)))
		{
			say("You may not redirect output to yourself");
			return;
		}
		window_redirect(to, from_server);
		server_list[from_server].sent = 0;
		parse_line((u_char *) 0, args, (u_char *) 0, 0, 0, 0);
		if (server_list[from_server].sent)
			send_to_server("%s", current_screen->redirect_token,
				current_screen->screennum);
		else
			window_redirect(NULL, from_server);
	}
	else
		say("Usage: REDIRECT <nick|channel|%process> <cmd>");
}
