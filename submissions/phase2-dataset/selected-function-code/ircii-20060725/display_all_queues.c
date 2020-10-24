static	void
display_all_queues(queue)
	Queue	*queue;
{
	Queue *tmp;

	if (!queue)
		return;

	tmp = queue->next;
	while (tmp)
	{
		print_queue(tmp);
		if (tmp->next == (Queue *) 0)
			return;
		else
			tmp = tmp->next;
	}
	yell("QUEUE: No more queues");
}
