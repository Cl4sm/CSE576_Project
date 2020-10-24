static	int
term_CS_scroll(line1, line2, n)
	int	line1,
		line2,
		n;
{
	int	i;
	u_char	*thing;

	if (n > 0)
		thing = UP(SF ? SF : NL);
	else if (n < 0)
	{
		if (SR)
			thing = UP(SR);
		else
			return 1;
	}
	else
		return 0;
	tputs_x(tgoto(CS, line2, line1));  /* shouldn't do this each time */
	if (n < 0)
	{
		term_move_cursor(0, line1);
		n = -n;
	}
	else
		term_move_cursor(0, line2);
	for (i = 0; i < n; i++)
		tputs_x(CP(thing));
	tputs_x(tgoto(CS, current_screen->li - 1, 0));	/* shouldn't do this each time */
	return (0);
}
