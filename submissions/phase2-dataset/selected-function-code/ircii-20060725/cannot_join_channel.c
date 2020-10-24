static	void
cannot_join_channel(from, ArgList)
	u_char	*from,	
		**ArgList;
{
	u_char	*chan;
	u_char	buffer[BIG_BUFFER_SIZE];

	if (ArgList[0])
		chan = ArgList[0];
	else
		return;

	if (!is_on_channel(chan, parsing_server_index,
			get_server_nickname(parsing_server_index)))
		remove_channel(chan, parsing_server_index);
	else
		return;

	PasteArgs(ArgList, 0);
	if (do_hook(current_numeric, "%s %s", from, *ArgList)) {
		my_strmcpy(buffer, ArgList[0], sizeof buffer);
		switch(-current_numeric)
		{
		case 471:
			my_strmcat(buffer, " (Channel is full)", sizeof buffer);
			break;
		case 473:
			my_strmcat(buffer, " (Invite only channel)", sizeof buffer);
			break;
		case 474:
			my_strmcat(buffer, " (Banned from channel)", sizeof buffer);
			break;
		case 475:
			my_strmcat(buffer, " (Bad channel key)", sizeof buffer);
			break;
		case 476:
			my_strmcat(buffer, " (Bad channel mask)", sizeof buffer);
			break;
		}
		put_it("%s %s", numeric_banner(), buffer);
	}
}
