static	void
parse_note(server, line)
	u_char	*server;
	u_char	*line;
{
	u_char	*date,
		*nick,
		*flags,
		*high,
		*name,
		*message;
	int	ign1,
		ign2,
		level;
	time_t	the_time;

	flags = next_arg(line, &date);	/* what to do with these flags */
	nick = next_arg(date, &date);
	name = next_arg(date, &date);
	if ((message = my_index(date, '*')) != NULL)
		*message = (u_char) 0;
	if (((ign1 = is_ignored(nick, IGNORE_NOTES)) == IGNORED) ||
			((ign2 = is_ignored(name, IGNORE_NOTES)) == IGNORED))
		return;
	if ((ign1 == HIGHLIGHTED) || (ign2 == HIGHLIGHTED))
		high = &highlight_char;
	else
		high = empty_string;
	the_time = my_atol(date);
	date = UP(ctime(&the_time));
	date[24] = (u_char) 0;
	level = set_lastlog_msg_level(LOG_NOTES);
	if (do_hook(NOTE_LIST, "%s %s %s %s %s %s", nick, name, flags, date,
		server, message + 2))
	{
		put_it("Note from %s (%s) %s", nick, name, flags);
		put_it("It was queued %s from server %s", date, server);
		put_it("%s[%s]%s %s", high, nick, high, message + 2);
	}
	if (beep_on_level & LOG_NOTES)
		beep_em(1);
	set_lastlog_msg_level(level);
}
