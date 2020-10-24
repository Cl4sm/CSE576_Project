static	IrcCommand *
find_command(com, cnt)
	u_char	*com;
	int	*cnt;
{
	size_t	len;

	if (com && (len = my_strlen(com)))
	{
		int	min,
			max,
			pos,
			old_pos = -1,
			c;

		min = 1;
		max = NUMBER_OF_COMMANDS + 1;
		while (1)
		{
			pos = (max + min) / 2;
			if (pos == old_pos)
			{
				*cnt = 0;
				return ((IrcCommand *) 0);
			}
			old_pos = pos;
			c = my_strncmp(com, irc_command[pos].name, len);
			if (c == 0)
				break;
			else if (c > 0)
				min = pos;
			else
				max = pos;
		}
		*cnt = 0;
		(*cnt)++;
		min = pos - 1;
		while ((min > 0) && (my_strncmp(com, irc_command[min].name,
				len) == 0))
		{
			(*cnt)++;
			min--;
		}
		min++;
		max = pos + 1;
		while ((max < NUMBER_OF_COMMANDS + 1) && (my_strncmp(com,
				irc_command[max].name, len) == 0))
		{
			(*cnt)++;
			max++;
		}
		if (*cnt)
		{
			if (my_strlen(irc_command[min].name) == len)
				*cnt *= -1;
			else if (*cnt == 1 && 
					irc_command[min].flags&NONOVICEABBREV &&
					get_int_var(NOVICE_VAR))
			{
				say("As a novice you may not abbreviate the %s command", irc_command[min].name);
				*cnt=0;
				return ((IrcCommand *) 0);
			}
			return (&(irc_command[min]));
		}
		else
			return ((IrcCommand *) 0);
	}
	else
	{
		*cnt = -1;
		return (irc_command);
	}
}
