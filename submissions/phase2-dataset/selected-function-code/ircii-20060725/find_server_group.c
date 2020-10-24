find_server_group(group, add)
	u_char	*group;
	int	add;
{
	static	int	next = 1;
	SGroup *g;

	if (!group || !*group)
		return 0;

	g = (SGroup *) find_in_list((List **)(void *)&server_group_list, group, 0);
	if (g)
		goto end;

	if (!add)
		return 0;

	g = (SGroup *) new_malloc(sizeof(SGroup));
	g->name = (u_char *) 0;
	malloc_strcpy(&g->name, group);
	g->number = next++;
	add_to_list((List **)(void *)&server_group_list, (List *) g);
end:
	return g->number;
}
