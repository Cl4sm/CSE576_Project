static	void
e_wall(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	save_message_from();
	if (my_strcmp(command, "WALL") == 0)
	{	/* I hate this */
		message_from(NULL, LOG_WALL);
		if (!get_server_operator(from_server))
			put_it("## %s", args);
	}
	else
	{
		message_from(NULL, LOG_WALLOP);
		if (!get_server_flag(from_server, USER_MODE_W))
			put_it("!! %s", args);
	}
	if (!in_on_who)
		send_to_server("%s :%s", command, args);
	restore_message_from();
}
