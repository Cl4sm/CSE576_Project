static	int
term_ALDL_scroll(line1, line2, n)
	int	line1,
		line2,
		n;
{
	int	i;

	if (n > 0)
	{
		term_move_cursor(0, line1);
		for (i = 0; i < n; i++)
			tputs_x(DL);
		term_move_cursor(0, line2 - n + 1);
		for (i = 0; i < n; i++)
			tputs_x(AL);
	}
	else if (n < 0)
	{
		n = -n;
		term_move_cursor(0, line2-n+1);
		for (i=0; i < n; i++)
			tputs_x(DL);
		term_move_cursor(0, line1);
		for (i=0; i < n; i++)
			tputs_x(AL);
	}
	return (0);
}
