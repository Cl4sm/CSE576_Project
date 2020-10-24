xtypecmd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*arg;
	size_t	len;

	if (args && *args == '-')
	{
		args++;
		if ((arg = next_arg(args, &args)) != NULL)
		{
			len = my_strlen(arg);
			if (!my_strnicmp(arg, UP("LITERAL"), len))
			{
				for (; *args; args++)
					input_add_character((u_int)*args, (u_char *) 0);
			}
#ifdef _Windows
			else if (!my_strnicmp(arg, UP("REPLACE"), len))
			{
				set_input(args);
				term_resetall();
			}
#endif /* _Windows */
			else
				say ("Unknown flag -%s to XTYPE", arg);
			return;
		}
		input_add_character('-', (u_char *) 0);
	}
	else
		typecmd(command, args, empty_string);
	return;
}
