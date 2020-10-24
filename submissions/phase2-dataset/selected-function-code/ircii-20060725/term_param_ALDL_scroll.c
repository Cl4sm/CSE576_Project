static	int
term_param_ALDL_scroll(line1, line2, n)
	int	line1,
		line2,
		n;
{
	if (n > 0)
	{
		term_move_cursor(0, line1);
		tputs_x(tgoto(DL, n, n));
		term_move_cursor(0, line2 - n + 1);
		tputs_x(tgoto(AL, n, n));
	}
	else if (n < 0)
	{
		n = -n;
		term_move_cursor(0, line2-n+1);
		tputs_x(tgoto(DL, n, n));
		term_move_cursor(0, line1);
		tputs_x(tgoto(AL, n, n));
	}
	return (0);
}
