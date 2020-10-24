static	u_char
display_bold(flag)
	int	flag;
{
	static	int	bold = OFF;

	term_flush();
	if (flag == bold)
		return (flag);
	switch (flag)
	{
	case ON:
		bold = ON;
		if (get_int_var(BOLD_VIDEO_VAR))
			term_bold_on();
		return (OFF);
	case OFF:
		bold = OFF;
		if (get_int_var(BOLD_VIDEO_VAR))
			term_bold_off();
		return (ON);
	case TOGGLE:
		if (bold == ON)
		{
			bold = OFF;
			if (get_int_var(BOLD_VIDEO_VAR))
				term_bold_off();
			return (ON);
		}
		else
		{
			bold = ON;
			if (get_int_var(BOLD_VIDEO_VAR))
				term_bold_on();
			return (OFF);
		}
	}
	return OFF;
}
