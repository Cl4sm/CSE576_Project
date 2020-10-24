static	int
show_numeric_list(numeric)
	int	numeric;
{
	NumericList *tmp;
	Hook	*list;
	u_char	buf[4];
	int	cnt = 0;

	if (numeric)
	{
		snprintf(CP(buf), sizeof buf, "%3.3u", numeric);
		if ((tmp = (NumericList *) find_in_list((List **)(void *)&numeric_list, buf, 0))
				!= NULL)
		{
			for (list = tmp->list; list; list = list->next, cnt++)
				show_hook(list, tmp->name);
		}
	}
	else
	{
		for (tmp = numeric_list; tmp; tmp = tmp->next)
		{
			for (list = tmp->list; list; list = list->next, cnt++)
				show_hook(list, tmp->name);
		}
	}
	return (cnt);
}
