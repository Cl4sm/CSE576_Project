display_msg(from, ArgList)
	u_char	*from,
		**ArgList;
{
	u_char	*ptr;
	u_char	*rest;

	rest = PasteArgs(ArgList, 0);
	if (from && (my_strnicmp(get_server_itsname(parsing_server_index), from,
			my_strlen(get_server_itsname(parsing_server_index))) == 0))
		from = (u_char *) 0;
	if ((ptr = (u_char *) my_index(rest, ':')) != NULL)
	{
		*(ptr++) = (u_char) 0;
		if (my_strlen(rest))
		{
			if (from)
				put_it("%s %s: %s (from %s)", numeric_banner(),
					rest, ptr, from);
			else
				put_it("%s %s: %s", numeric_banner(), rest,
					ptr);
		}
		else
		{
			if (from)
				put_it("%s %s (from %s)", numeric_banner(),
					ptr, from);
			else
				put_it("%s %s", numeric_banner(), ptr);
		}
	}
	else
	{
		if (from)
			put_it("%s %s (from %s)", numeric_banner(), rest, from);
		else
			put_it("%s %s", numeric_banner(), rest);
	}
}
