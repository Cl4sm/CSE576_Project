clear_channel_list(int server)
{
	ChannelList *tmp,
		*next;
	Window		*ptr;
	int		flag = 1;

	while ((ptr = traverse_all_windows(&flag)))
		if (ptr->server == server && ptr->current_channel)
			new_free(&ptr->current_channel);
	
	for (tmp = server_list[server].chan_list; tmp; tmp = next)
	{
		next = tmp->next;
		free_channel(&tmp);
	}
	server_list[server].chan_list = (ChannelList *) 0;
	return;
}
