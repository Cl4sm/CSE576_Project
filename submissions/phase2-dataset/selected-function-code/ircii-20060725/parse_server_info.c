void
parse_server_info(name, port, password, nick, group, extra, type)
	u_char	**name,
		**port,
		**password,
		**nick,
		**group,
		**extra;
	int	*type;
{
	u_char *ptr, *ename, *savename = (u_char *) 0;

	*port = *password = *nick = *extra = NULL;
	if (my_strncmp(*name, "IRC/", 4) == 0)
	{
		*type = DEFAULT_SERVER_VERSION;
		*name += 4;
	}
	else
	if (my_strncmp(*name, "ICB/", 4) == 0)
	{
		*type = ServerICB;
		*name += 4;
	}

	/* check for :group: processing */
	if (**name == ':')
	{
		if ((ename = my_index((*name)+1, ':')))
		{
			*ename = '\0';
			if (group)
				*group = *name + 1;
			*name = ename + 1;	/* now points to empty or : we hope */
		}
	}

	/* check for [i:p:v:6]:port style */
	if (**name == '[')
	{
		if ((ename = my_index((*name)+1, ']')))
		{
			*ename = '\0';
			savename = *name + 1;
			*name = ename + 1;	/* now points to empty or : we hope */
		}
	}

	if ((ptr = my_index(*name, ':')) != NULL)
	{
		*(ptr++) = '\0';
		if (my_strlen(ptr) == 0)
			*port = (u_char *) 0;
		else
		{
			*port = ptr;
			if ((ptr = my_index(ptr, ':')) != NULL)
			{
				*(ptr++) = '\0';
				if (my_strlen(ptr) == 0)
					*password = '\0';
				else
				{
					*password = ptr;
					if ((ptr = my_index(ptr, ':'))
							!= NULL)
					{
						*(ptr++) = '\0';
						if (!my_strlen(ptr))
							*nick = NULL;
						else
						{
							*nick = ptr;
							if (extra && (ptr = my_index(ptr, ':'))
									!= NULL)
							{
								*(ptr++) = '\0';
								if (!my_strlen(ptr))
									*extra = NULL;
								else
									*extra = ptr;
							}
						}
					}
				}
			}
		}
	}
	if (savename)
		*name = savename;
}
