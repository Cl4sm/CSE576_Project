create_refnum()
{
	unsigned int	new_refnum = 1;
	Window	*tmp;
	int	done = 0,
		flag;

	while (!done)
	{
		done = 1;
		if (new_refnum == 0)
			new_refnum++;

		flag = 1;
		while ((tmp = traverse_all_windows(&flag)) != NULL)
		{
			if (tmp->refnum == new_refnum)
			{
				done = 0;
				new_refnum++;
				break;
			}
		}
	}
	return (new_refnum);
}
