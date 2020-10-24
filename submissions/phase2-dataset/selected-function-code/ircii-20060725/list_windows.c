static	void
list_windows()
{
	Window	*tmp;
	int	flag = 1;
	int	len = 4;
	int	clen = get_int_var(CHANNEL_NAME_WIDTH_VAR);
	int	check_clen = clen == 0;

	while ((tmp = traverse_all_windows(&flag)) != NULL)
	{
		if (tmp->name && ((int) my_strlen(tmp->name) > len))
			len = my_strlen(tmp->name);
		if (check_clen == 0)
			continue;
		if (tmp->current_channel &&
		    ((int) my_strlen(tmp->current_channel) > clen))
			clen = my_strlen(tmp->current_channel);
	}
	if (do_hook(WINDOW_LIST_LIST, WIN_FORM_HOOK, len,
		    "Ref", "Nick", "Name", "Channel", "Query",
		    "Server", "Level", empty_string))
	{
		u_char	buffer[BIG_BUFFER_SIZE];

		/* XXX: 9 is old NICKNAME_LEN */
		snprintf(CP(buffer), sizeof buffer, WIN_FORM, 9, 9, len, len, clen,
		    clen);
		say(CP(buffer),
		    "Ref", "Nick", "Name", "Channel", "Query",
		    "Server", "Level", empty_string);
	}
	flag = 1;
	while ((tmp = traverse_all_windows(&flag)) != NULL)
		list_a_window(tmp, len, clen);
}
