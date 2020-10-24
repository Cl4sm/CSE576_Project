parse_lastlog_level(str)
	u_char	*str;
{
	u_char	*ptr,
		*rest,
		*s;
	int	i,
		p,
		level,
		neg;
	size_t	len;

	level = 0;
	while ((str = next_arg(str, &rest)) != NULL)
	{
		while (str)
		{
			if ((ptr = my_index(str, ',')) != NULL)
				*ptr++ = '\0';
			if ((len = my_strlen(str)) != 0)
			{
				u_char	*cmd = NULL;

				malloc_strcpy(&cmd, str);
				upper(cmd);
				if (my_strncmp(cmd, "ALL", len) == 0)
					level = LOG_ALL;
				else if (my_strncmp(cmd, "NONE", len) == 0)
					level = 0;
				else
				{
					if (*str == '-')
					{
						str++;
						s = cmd + 1;
						neg = 1;
					}
					else {
						neg = 0;
						s = cmd;
					}
					for (i = 0, p = 1; i < NUMBER_OF_LEVELS; i++, p <<= 1)
					{
						if (!my_strncmp(s, levels[i], len))
						{
							if (neg)
								level &= (LOG_ALL ^ p);
							else
								level |= p;
							break;
						}
					}
					if (i == NUMBER_OF_LEVELS)
						say("Unknown lastlog level: %s",
							str);
				}
				new_free(&cmd);
			}
			str = ptr;
		}
		str = rest;
	}
	return (level);
}
