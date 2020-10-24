static	void
e_channel(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	*chan;
	size_t	len;
	u_char	*chanstr = (u_char *) 0,
		*ptr;
	int 	force = 0;

	if (get_server_version(from_server) == Server2_5)
		command = UP("CHANNEL");
	save_message_from();
	message_from((u_char *) 0, LOG_CURRENT);		/* XXX should delete this */
	if ((chan = next_arg(args, &args)) != NULL)
	{
		len = MAX(2, my_strlen(chan));
		if (my_strnicmp(chan, UP("-force"), len) == 0)
		{
			force = 1;
			if ((chan = next_arg(args, &args)) == NULL)
				goto out;	/* XXX: allow /alias join join -force */
			len = MAX(2, my_strlen(chan));
		}
		if (my_strnicmp(chan, UP("-invite"), len) == 0)
		{
			if (invite_channel)
			{
				if ((ptr = do_channel(invite_channel, force)))
				{
					if (get_server_version(from_server) == ServerICB)
						icb_put_group(invite_channel);
					else
						send_to_server("%s %s %s", command, invite_channel, args);
				}
				else
					say("You are already on %s ?", invite_channel);
			}
			else
				say("You have not been invited to a channel!");
		}
		else if (get_server_version(from_server) == ServerICB)
		{
			if (do_channel(chan, force))
				icb_put_group(chan);
		}
		else
		{
			malloc_strcpy(&chanstr, chan);
			chan = chanstr;

			ptr = my_strsep(&chanstr, UP(","));
			if ((ptr = do_channel(ptr, force)) && *ptr)
				send_to_server("%s %s %s", command, ptr, args);

			while (get_int_var(NOVICE_VAR) == 0 && (ptr = my_strsep(&chanstr, UP(","))))
				if ((ptr = do_channel(ptr, force)) && *ptr)
					send_to_server("%s %s %s", command, ptr, args);

			new_free(&chan);
		}
	}
	else
out:
		list_channels();
	restore_message_from();
}
