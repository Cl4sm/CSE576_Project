funny_list(from, ArgList)
	u_char	*from;
	u_char	**ArgList;
{
	u_char	*channel,
		*user_cnt,
		*line;
	WideList **new_list;
	int	cnt;
	static	u_char	format[25];
	static	int	last_width = -1;

	if (last_width != get_int_var(CHANNEL_NAME_WIDTH_VAR))
	{
		if ((last_width = get_int_var(CHANNEL_NAME_WIDTH_VAR)) != 0)
			snprintf(CP(format), sizeof format, "*** %%-%u.%us %%-5s  %%s",
				(u_char) last_width,
				(u_char) last_width);
		else
			my_strmcpy(format, "*** %s\t%-5s  %s", sizeof format);
	}
	channel = ArgList[0];
	user_cnt = ArgList[1];
	line = PasteArgs(ArgList, 2);
	if (funny_flags & FUNNY_TOPIC && !(line && *line))
			return;
	cnt = my_atoi(user_cnt);
	if (funny_min && (cnt < funny_min))
		return;
	if (funny_max && (cnt > funny_max))
		return;
	if ((funny_flags & FUNNY_PRIVATE) && (*channel != '*'))
		return;
	if ((funny_flags & FUNNY_PUBLIC) && (*channel == '*'))
		return;
	if (match_str)
	{
		if (wild_match(match_str, channel) == 0)
			return;
	}
	if (funny_flags & FUNNY_WIDE)
	{
		if (wl_elements >= wl_size)
		{
			new_list = (WideList **) new_malloc(sizeof(WideList *) *
			    (wl_size + 50));
			bzero(new_list, sizeof(WideList *) * (wl_size + 50));
			if (wl_size)
				bcopy(wide_list, new_list, sizeof(WideList *)
					* wl_size);
			wl_size += 50;
			new_free(&wide_list);
			wide_list = new_list;
		}
		wide_list[wl_elements] = (WideList *)
			new_malloc(sizeof(WideList));
		wide_list[wl_elements]->channel = (u_char *) 0;
		wide_list[wl_elements]->users = cnt;
		malloc_strcpy(&wide_list[wl_elements]->channel,
				(*channel != '*') ? channel : (u_char *) "Prv");
		wl_elements++;
		return;
	}
	if (do_hook(current_numeric, "%s %s %s %s", from,  channel, user_cnt,
	    line) && do_hook(LIST_LIST, "%s %s %s", channel, user_cnt, line))
	{
		if (channel && user_cnt)
		{
			if (*channel == '*')
				put_it(CP(format), "Prv", user_cnt, line);
			else
				put_it(CP(format), channel, user_cnt, line);
		}
	}
}
