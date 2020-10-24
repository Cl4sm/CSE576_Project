static	void
cd(command, args, subargs)
	u_char	*command,
		*args,
		*subargs;
{
	u_char	lbuf[BIG_BUFFER_SIZE];
	u_char	*arg,
		*expand;

#ifdef DAEMON_UID
	if (getuid() == DAEMON_UID)
	{
		say("You are not permitted to use this command");
		return;
	}
#endif /* DAEMON_UID */
	if ((arg = next_arg(args, &args)) != NULL)
	{
		if ((expand = expand_twiddle(arg)) != NULL)
		{
			if (chdir(CP(expand)))
				say("CD: %s", strerror(errno));
			new_free(&expand);
		}
		else
			say("CD: No such user");
	}
	getcwd(CP(lbuf), sizeof(lbuf));
	say("Current directory: %s", lbuf);
}
