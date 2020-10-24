int
unhold_windows()
{
	Window	*tmp;
	u_char	*stuff;
	int	hold_flag = 0,
		flag = 1;
	int	logged;

	while ((tmp = traverse_all_windows(&flag)) != NULL)
	{
		if (!hold_output(tmp) && (stuff = hold_queue(tmp)))
		{
			logged = hold_queue_logged(tmp);
			if (rite(tmp, stuff, 1, 0, 0, logged) == 0)
			{
				remove_from_hold_list(tmp);
				hold_flag = 1;
			}
		}
	}
	return (hold_flag);
}
