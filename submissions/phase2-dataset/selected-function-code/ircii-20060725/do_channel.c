do_channel(chan, force)
	u_char	*chan;
	int force;
{
	ChannelList	*channel;
	u_char		*old;

	if (from_server < 0 || curr_scr_win->server < 0)
		return (u_char *) 0;

	channel = lookup_channel(chan, curr_scr_win->server, CHAN_NOUNLINK);

	if (is_bound(chan, curr_scr_win->server) && channel && channel->window != curr_scr_win)
		say("Channel %s is bound", chan);
	else if (is_on_channel(chan, curr_scr_win->server, get_server_nickname(curr_scr_win->server)) ||
	    (get_server_version(from_server) == ServerICB && my_stricmp(get_server_icbgroup(from_server), chan) == 0))
	{
		is_current_channel(chan, curr_scr_win->server, 1);
		say("You are now talking to channel %s", set_channel_by_refnum(0, chan));
		update_all_windows();
	}
	else
	{
		/* only do this if we're actually joining a new channel */
		if (get_int_var(NOVICE_VAR) && get_server_version(from_server) != ServerICB)
		{
			if ((old = get_channel_by_refnum(0)) && my_strcmp(old, zero))
				send_to_server("PART %s", old);
		}
		add_channel(chan, 0, from_server, CHAN_JOINING, (ChannelList *) 0);
		force = 1;
	}
	if (force)
		return chan;
	else
		return (u_char *) 0;
}
