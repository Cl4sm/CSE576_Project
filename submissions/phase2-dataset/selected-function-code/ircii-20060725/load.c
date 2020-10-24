load(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	FILE	*fp;
	u_char	*filename,
		*expanded = NULL;
	int	flag = 0;
	struct	stat	stat_buf;
	int	paste_level = 0;
	u_char	*start,
		*current_row = NULL,
		lbuf[BIG_BUFFER_SIZE];
	int	no_semicolon = 1;
	u_char	*ircpath;
	int	display;
#ifdef ZCAT
	u_char	*expand_z = NULL;
	int	exists;
	int	pos;
#endif /* ZCAT */

	ircpath = get_string_var(LOAD_PATH_VAR);
	if (!ircpath)
	{
		say("LOAD_PATH has not been set");
		return;
	}

	if (load_depth == MAX_LOAD_DEPTH)
	{
		say("No more than %d levels of LOADs allowed", MAX_LOAD_DEPTH);
		return;
	}
	load_depth++;
	status_update(0);
#ifdef DAEMON_UID
	if (getuid() == DAEMON_UID)
	{
		say("You may only load your SAVED file");
		filename = ircrc_file;
	}
	else
#endif /* DAEMON_UID */
		while ((filename = next_arg(args, &args)) != NULL)
		{
			if (my_strnicmp(filename, UP("-args"), my_strlen(filename)) == 0)
				flag = 1;
			else
				break;
		}
	if (filename)
	{
		if ((expanded = expand_twiddle(filename)) != NULL)
		{
#ifdef ZCAT
			/* Handle both <expanded> and <expanded>.Z */
			pos = my_strlen(expanded) - my_strlen(ZSUFFIX);
			if (pos < 0 || my_strcmp(expanded + pos, ZSUFFIX))
			{
				malloc_strcpy(&expand_z, expanded);
				malloc_strcat(&expand_z, UP(ZSUFFIX));
			}
#endif /* ZCAT */
			if (*expanded != '/')
			{
				filename = path_search(expanded, ircpath);
#ifdef ZCAT
				if (!filename && expand_z)
					filename = path_search(expand_z, ircpath);
#endif /* ZCAT */
				if (!filename)
				{
					say("%s: File not found", expanded);
					status_update(1);
					load_depth--;
#ifdef ZCAT
					new_free(&expand_z);
#endif /* ZCAT */
					new_free(&expanded);
					return;
				}
				else
					malloc_strcpy(&expanded, filename);
			}
#ifdef ZCAT
			if ((exists = stat(CP(expanded), &stat_buf)) == -1)
			{
				if (!(exists = stat(CP(expand_z), &stat_buf)))
				{
					if (expanded)
						new_free(&expanded);
					expanded = expand_z;
				}
				else
					new_free(&expand_z);
			}
			if (exists == 0)
#else
				if (!stat(expanded, &stat_buf))
#endif /* ZCAT */
				{
					if (stat_buf.st_mode & S_IFDIR)
					{
						say("%s is a directory", expanded);
						status_update(1);
						load_depth--;
#ifdef ZCAT
						new_free(&expand_z);
#endif /* ZCAT */
						new_free(&expanded);
						return;
					}
					/* sigh.  this is lame */
#if defined(S_IXUSR) && defined(S_IXGRP) && defined(S_IXOTH)
# define IS_EXECUTABLE (S_IXUSR|S_IXGRP|S_IXOTH)
#else
# define IS_EXECUTABLE 0111
#endif /* S_IXUSR && S_IXGRP && S_IXOTH */
					if (stat_buf.st_mode & IS_EXECUTABLE)
					{
						yell("*** %s is executable and may not be loaded", expanded);
						status_update(1);
						load_depth--;
#ifdef ZCAT
						new_free(&expand_z);
#endif /* ZCAT */
						new_free(&expanded);
						return;
					}
				}
			if (command && *command == 'W')
			{
				say("%s", expanded);
				status_update(1);
				load_depth--;
				new_free(&expanded);
#ifdef ZCAT
				new_free(&expand_z);
#endif /* ZCAT */
				return;
			}
#ifdef ZCAT
			/* Open if uncompressed, zcat if compressed */
			pos = my_strlen(expanded) - my_strlen(ZSUFFIX);
			if (pos >= 0 && !my_strcmp(expanded + pos, ZSUFFIX))
				fp = zcat(expanded);
			else
				fp = fopen(CP(expanded), "r");
			if (fp != NULL)
#else
				if (fp = fopen(CP(expanded), "r"))
#endif /* ZCAT */
				{
					display = window_display;
					window_display = 0;
					current_row = NULL;
					if (!flag)
						args = NULL;
					for (;;)
					{
						if (fgets(CP(lbuf), sizeof(lbuf) / 2, fp))	/* XXX need better /load policy, but this will do for now */
	{
		size_t	len;
		u_char	*ptr;

		for (start = lbuf; isspace(*start); start++)
			;
		if (!*start || *start == '#')
			continue;

		len = my_strlen(start);
	/*
	 * this line from stargazer to allow \'s in scripts for continued
	 * lines <spz@specklec.mpifr-bonn.mpg.de>
	 */
		while (start[len-1] == '\n' && start[len-2] == '\\' &&
		    len < sizeof(lbuf) / 2 && fgets(CP(&(start[len-2])),
		    (int)(sizeof(lbuf) / 2 - len), fp))
			len = my_strlen(start);

		if (start[len - 1] == '\n')
		    start[--len] = '\0';

		while (start && *start)
		{
			u_char	*optr = start;
			while ((ptr = sindex(optr, UP("{};"))) &&
					ptr != optr &&
					ptr[-1] == '\\')
				optr = ptr+1;

			if (no_semicolon)
				no_semicolon = 0;
			else if ((!ptr || ptr != start) && current_row)
			{
				if (!paste_level)
				{
					parse_line(NULL, current_row,
						args, 0, 0, 0);
					new_free(&current_row);
				}
				else
					malloc_strcat(&current_row, UP(";"));
			}

			if (ptr)
			{
				u_char	c = *ptr;

				*ptr = '\0';
				malloc_strcat(&current_row, start);
				*ptr = c;

				switch (c)
				{
				case '{' :
					paste_level++;
					if (ptr == start)
						malloc_strcat(&current_row, UP(" {"));
					else
						malloc_strcat(&current_row, UP("{"));
					no_semicolon = 1;
					break;

				case '}' :
					if (!paste_level)
						yell("Unexpected }");
					else
					{
						--paste_level;
						malloc_strcat(&current_row, UP("}"));
						no_semicolon = ptr[1] ? 1 : 0;
					}
					break;

				case ';' :
					malloc_strcat(&current_row, UP(";"));
					no_semicolon = 1;
					break;
				}

				start = ptr+1;
			}
			else
			{
				malloc_strcat(&current_row, start);
				start = NULL;
			}
		}
	}
						else
							break;
					}
					if (current_row)
					{
						if (paste_level)
							yell("Unexpected EOF");
						else
							parse_line(NULL,
								current_row, 
								args, 0, 0, 0);
						new_free(&current_row);
					}
					window_display = display;
					fclose(fp);
				}
				else
					say("Couldn't open %s: %s", expanded,
						strerror(errno));
			new_free(&expanded);
#ifdef ZCAT
			new_free(&expand_z);
#endif /* ZCAT */
		}
		else
			say("Unknown user");
	}
	else
		say("No filename specified");
	status_update(1);
	load_depth--;
}
