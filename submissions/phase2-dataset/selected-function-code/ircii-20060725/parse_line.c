void
parse_line(name, org_line, args, hist_flag, append_flag, eat_space)
	u_char	*name,
		*org_line,
		*args;
	int	hist_flag,
		append_flag,
		eat_space;
{
	u_char	*line = NULL,
		*free_line, *stuff, *start, *lbuf, *s, *t;
	int	args_flag;

	malloc_strcpy(&line, org_line);
	free_line = line;
	args_flag = 0;
	if (!*line)
		do_send_text(NULL, empty_string, empty_string);
	else if (args)
		do
		{
			stuff = expand_alias(name, line, args, &args_flag,
					&line);
			start = stuff;
			if (eat_space)
				for (; isspace(*start); start++)
					;       

			if (!line && append_flag && !args_flag && args && *args)
			{
				lbuf = (u_char *) new_malloc(my_strlen(stuff) + 1 + my_strlen(args) + 1);
				my_strcpy(lbuf, start);
				my_strcat(lbuf, " ");
				my_strcat(lbuf, args);
				new_free(&stuff);
				start = stuff = lbuf;
			}
			parse_command(start, hist_flag, args);
			new_free(&stuff);
		}
		while (line);
	else
	{
		start = line;
		if (eat_space)
			for (; isspace(*start); start++)
				;
		if (load_depth)
			parse_command(start, hist_flag, args);
		else
			while ((s = line))
			{
				if ((t = sindex(line, UP("\r\n"))) != NULL)
				{
					*t++ = '\0';
					if (eat_space)
						for (; isspace(*t); t++)
							;
					line = t;
				}
				else
					line = NULL;
				parse_command(s, hist_flag, args);
			}
	}
	new_free(&free_line);
	return;
}
