bits_to_lastlog_level(level)
	int	level;
{
	static	u_char	lbuf[128]; /* this *should* be enough for this */
	int	i,
		p;
	int	first = 1;

	if (level == LOG_ALL)
		my_strcpy(lbuf, "ALL");
	else if (level == 0)
		my_strcpy(lbuf, "NONE");
	else
	{
		*lbuf = '\0';
		for (i = 0, p = 1; i < NUMBER_OF_LEVELS; i++, p <<= 1)
		{
			if (level & p)
			{
				if (first)
					first = 0;
				else
					my_strmcat(lbuf, " ", 127);
				my_strmcat(lbuf, levels[i], 127);
			}
		}
	}
	return (lbuf);
}
