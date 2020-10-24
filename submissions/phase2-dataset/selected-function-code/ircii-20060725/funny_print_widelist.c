funny_print_widelist()
{
	int	i;
	u_char	buffer1[BIG_BUFFER_SIZE];
	u_char	buffer2[BIG_BUFFER_SIZE];

	if (!wide_list)
		return;

	if (funny_flags & FUNNY_NAME)
		qsort((void *) wide_list, wl_elements, sizeof(WideList *),
			(int (*)(const void *, const void *)) funny_widelist_names);
	else if (funny_flags & FUNNY_USERS)
		qsort((void *) wide_list, wl_elements, sizeof(WideList *),
			(int (*)(const void *, const void *)) funny_widelist_users);

	*buffer1 = '\0';
	for (i = 0; i < wl_elements; i++)
	{
		snprintf(CP(buffer2), sizeof buffer2, "%s(%d) ", wide_list[i]->channel,
				wide_list[i]->users);
		if (my_strlen(buffer1) + my_strlen(buffer2) > current_screen->co - 5)
		{
			if (do_hook(WIDELIST_LIST, "%s", buffer1))
				say("%s", buffer1);
			*buffer1 = '\0';
		}
		my_strmcat(buffer1, buffer2, sizeof buffer1);
	}
	if (*buffer1 && do_hook(WIDELIST_LIST, "%s", buffer1))
		say("%s" , buffer1);
	for (i = 0; i < wl_elements; i++)
	{
		new_free(&wide_list[i]->channel);
		new_free(&wide_list[i]);
	}
	new_free(&wide_list);
	wl_elements = wl_size = 0;
}
