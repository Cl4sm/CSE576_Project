display_highlight(flag)
	int	flag;
{
	static	int	highlight = OFF;

	term_flush();
	if (flag == highlight)
		return (flag);
	switch (flag)
	{
	case ON:
		highlight = ON;
		if (get_int_var(INVERSE_VIDEO_VAR))
			term_standout_on();
		return (OFF);
	case OFF:
		highlight = OFF;
		if (get_int_var(INVERSE_VIDEO_VAR))
			term_standout_off();
		return (ON);
	case TOGGLE:
		if (highlight == ON)
		{
			highlight = OFF;
			if (get_int_var(INVERSE_VIDEO_VAR))
				term_standout_off();
			return (ON);
		}
		else
		{
			highlight = ON;
			if (get_int_var(INVERSE_VIDEO_VAR))
				term_standout_on();
			return (OFF);
		}
	}
	return flag;
}
