u_char *
get_notify_list(which)
	int which;
{
	u_char	*list = (u_char *) 0;
	NotifyList	*tmp;
	int first = 0;

	malloc_strcpy(&list, empty_string);
	for (tmp = notify_list; tmp; tmp = tmp->next)
	{
		if ((which & NOTIFY_LIST_ALL) == NOTIFY_LIST_ALL ||
		   ((which & NOTIFY_LIST_HERE) && tmp->flag) ||
		   ((which & NOTIFY_LIST_GONE) && !tmp->flag))
		{
			if (first++)
				malloc_strcat(&list, UP(" "));
			malloc_strcat(&list, tmp->nick);
		}
	}
	return list;
}
