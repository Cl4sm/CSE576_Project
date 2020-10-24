static int
switch_channels_backend(ChannelList * chan)
{
	u_char *newchan;

	for (; chan; chan = chan->next)
	{
		newchan = chan->channel;
		if (!is_current_channel(newchan, from_server, 0)
		    && !(is_bound(newchan, from_server)
			 && curr_scr_win != chan->window)
		    && (get_int_var(SWITCH_TO_QUIET_CHANNELS)
		        || !(chan->mode & MODE_QUIET)))
		{
			set_channel_by_refnum(0, newchan);
			update_all_windows();
			return 1;
		}
	}
	return 0;
}
