check_wait_command(nick)
	u_char 	*nick;
{
	if (waiting && !my_strcmp(nick, lame_wait_nick))
	{
		irc_io_loop = 0;
		return 1;
	}
	if (start_wait_list && !my_strcmp(nick, wait_nick))
	{
		if (start_wait_list->stuff)
		{
			parse_command(start_wait_list->stuff, 0, empty_string);
			new_free(&start_wait_list->stuff);
		}
		start_wait_list = start_wait_list->next;
		return 1;
	}
	return 0;
}
