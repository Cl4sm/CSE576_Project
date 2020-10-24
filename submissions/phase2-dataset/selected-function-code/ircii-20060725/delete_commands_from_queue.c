static	int
delete_commands_from_queue(queue, which)
	Queue	*queue;
	int	which;
{
	CmdList *ctmp = queue->first;
	CmdList *blah;
	int x;

	if (which == 1)
		queue->first = ctmp->next;
	else
	{
		for (x=1;x<which-1;x++)
		{
			if (ctmp->next) 
				ctmp = ctmp->next;
			else 
				return 0;
		}
		blah = ctmp->next;
		ctmp->next = ctmp->next->next;
		ctmp = blah;
	}
	new_free(&ctmp->what);
	new_free(&ctmp);
	if (queue->first == (CmdList *) 0)
		return 1;
	else
		return 0;
}
