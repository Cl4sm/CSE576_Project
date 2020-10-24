void
process_dcc(args)
	u_char	*args;
{
	u_char	*command;
	int	i;
	size_t	len;

	if (!(command = next_arg(args, &args)))
		return;
	len = my_strlen(command);
	upper(command);
	for (i = 0; dcc_commands[i].name != NULL; i++)
	{
		if (!my_strncmp(dcc_commands[i].name, command, len))
		{
			if (len < dcc_commands[i].uniq)
			{
				say("DCC command not unique: %s", command );
				return;
			}
			save_message_from();
			message_from((u_char *) 0, LOG_DCC);
			dcc_commands[i].function(args);
			restore_message_from();
			return;
		}
	}
	say("Unknown DCC command: %s", command);
}
