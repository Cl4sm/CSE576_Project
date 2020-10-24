recreate_mode(ChannelList *chan)
{
	int	mode_pos = 0,
		mode,
		showkey;
	static	u_char	*s;
	u_char	buffer[BIG_BUFFER_SIZE],
		modes[33],
		limit[33];

	limit[0] = '\0';	
	s = modes;
	mode = chan->mode;
	while (mode)
	{
		if (mode % 2)
			*s++ = mode_str[mode_pos];
		mode /= 2;
		mode_pos++;
	}
	*s = '\0';
	showkey = chan->key && *chan->key && !get_int_var(HIDE_CHANNEL_KEYS_VAR);
	if (chan->limit)
		snprintf(CP(limit), sizeof limit, " %d", chan->limit);

	snprintf(CP(buffer), sizeof buffer, "%s%s%s%s",
		 modes,
		 showkey ? UP(" ") : empty_string,
		 showkey ? chan->key : empty_string,
		 limit);

	malloc_strcpy(&chan->s_mode, buffer);
	return chan->s_mode;
}
