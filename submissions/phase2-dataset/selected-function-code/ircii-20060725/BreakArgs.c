static	void
BreakArgs(Input, Sender, OutPut)
	u_char	*Input;
	u_char	**Sender;
	u_char	**OutPut;
{
	u_char	*s = Input,
		*t;
	int	ArgCount = 0;

	/*
	 * Get sender from :sender and user@host if :nick!user@host
	 */
	FromUserHost = (u_char *) 0;

	if (*Input == ':')
	{
		u_char	*tmp;

		*Input++ = '\0';
		if ((s = (u_char *) my_index(Input, ' ')) != (u_char *) 0)
			*s++ = '\0';
		*Sender = Input;
		if ((tmp = (u_char *) my_index(*Sender, '!')) != (u_char *) 0)
		{
			*tmp++ = '\0';
			FromUserHost = tmp;
		}
	}
	else
		*Sender = empty_string;

	if (!s)
		return;

	for (;;)
	{
		while (*s == ' ')
			*s++ = '\0';

		if (!*s)
			break;

		if (*s == ':')
		{
			for (t = s; *t; t++)
				*t = *(t + 1);
			OutPut[ArgCount++] = s;
			break;
		}
		OutPut[ArgCount++] = s;
		if (ArgCount >= MAXPARA)
			break;

		for (; *s != ' ' && *s; s++)
			;
	}
	OutPut[ArgCount] = (u_char *) 0;
}
