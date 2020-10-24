window_set_server(refnum, server, misc)
	int	refnum;
	int	server;
	int	misc;
{
	int	old_serv;
	Window	*window = 0, *ptr, *new_win = (Window *) 0;
	ChannelList *tmp;
	int	moved = 0;
	int	flag = 1;

	if (refnum == -1)
	{
		old_serv = primary_server;
		primary_server = server;
		misc |= WIN_ALL;
	}
	else
	{
		window = get_window_by_refnum((u_int)refnum);
		old_serv = window->server;
	}

	if (server == old_serv)
		return;

	/* Moving all windows associated with old_serv -Sol */
	if (misc & WIN_ALL)
	{
		if ((misc & WIN_TRANSFER) && (old_serv >= 0))
		{
			for (tmp = server_list[old_serv].chan_list; tmp; tmp = tmp->next)
			{
				/* XXX: moved is always 0 at this point */
				if (!moved)	/* If we're here, it means
						   we're going to transfer
						   channels to the new server,
						   so we dump old channels
						   first, but only once -Sol */
				{
					moved++;
					clear_channel_list(server);
				}
				add_channel(tmp->channel, 0, server, CHAN_LIMBO, tmp);
			}
#ifdef NON_BLOCKING_CONNECTS
			if (server_list[old_serv].flags & CLOSE_PENDING)
				server_list[old_serv].flags |= CLEAR_PENDING;
			else
#endif /* NON_BLOCKING_CONNECTS */
				clear_channel_list(old_serv);
		}
		while ((ptr = traverse_all_windows(&flag)) != (Window *) 0)
			if (ptr->server == old_serv)
			{
				ptr->server = server;
				/*
				 * XXX we could save this to old_current_channel and use
				 * that after other checks to decide where a channel should
				 * go, maybe??
				 */
				if (ptr->current_channel)
					new_free(&ptr->current_channel);
			}
		window_check_servers();
		return;
	}

	/*
	 * We are setting only some windows of the old server : let's look
	 * for a window of that server that is not being moved.
	 * refnum == -1 has been dealt with above so window is defined. -Sol
	 */

	flag = 1;
	while ((ptr = traverse_all_windows(&flag)) != (Window *) 0)
		if ((ptr != window) && (!ptr->server_group || (ptr->server_group != window->server_group)) && (ptr->server == old_serv))
		{
			/* Possible relocation -Sol */
			new_win = ptr;

			/* Immediately retain window if no group -Sol */
			if (!ptr->server_group)
				break;
		}

	/* No relocation : we're closing last windows for old_serv -Sol */
	if (!new_win)
	{
		window_set_server(refnum, server, misc | WIN_ALL);
		return;
	}

	/*
	 * Now that we know that server still has at least one window open,
	 * move what we're supposed to -Sol
	 */

	if ((misc & WIN_TRANSFER) && (old_serv >= 0) && get_server_version(old_serv) != ServerICB)
		for (tmp = server_list[old_serv].chan_list; tmp; tmp = tmp->next)
			if ((tmp->window == window) || (window->server_group && (tmp->window->server_group == window->server_group)))
			{
				/* Found a channel to be relocated -Sol */
				if (tmp->window->sticky || (misc & WIN_FORCE))
				{	/* This channel moves -Sol */
					int	old = from_server;

					if (!moved)
					{
						moved++;
						clear_channel_list(server);
					}
					add_channel(tmp->channel, 0, server, CHAN_LIMBO, tmp); /* Copy it -Sol */
					from_server = old_serv; /* On old_serv,
								   leave it
								   -Sol */
					send_to_server("PART %s", tmp->channel);
					from_server = old;
					remove_channel(tmp->channel, old_serv);
				}
				else
					tmp->window = new_win;
			}

	flag = 1;
	while ((ptr = traverse_all_windows(&flag)) != (Window *) 0)
		if ((ptr == window) || (ptr->server_group && (ptr->server_group == window->server_group)))
		{
			ptr->server = server;
			if (ptr->current_channel)
				new_free(&ptr->current_channel);
		}
		window_check_servers();
}
