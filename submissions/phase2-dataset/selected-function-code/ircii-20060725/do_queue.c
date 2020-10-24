static	Queue	*
do_queue(queue, noflush)
	Queue	*queue;
	int	noflush;
{
	CmdList	*tmp;
	
	tmp = queue->first;
	
	do
	{
		if (tmp->what != (u_char *) 0)
			parse_line((u_char *) 0, tmp->what, empty_string, 0, 0, 0);
		tmp = tmp->next;
	}
	while (tmp != (CmdList *) 0);

	if (!noflush) 
		return remove_a_queue(queue);
	else
		return queue;
}
