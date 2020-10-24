static	int
connect_to_server_process(server_name, port, nick, server_index)
	u_char	*server_name;
	int	port;
	u_char	*nick;
	int	server_index;
{
#ifdef _Windows
	return -1;
#else
	int	write_des[2],
		read_des[2],
		pid,
		c;
	u_char	*path,
		*name = (u_char *) 0,
		*s;
	u_char	buffer[BIG_BUFFER_SIZE];
	int	old_timeout;

	path = UP(IRCIO_PATH);
	if ((s = my_rindex(path, '/')) != NULL)
		malloc_strcpy(&name, s + 1);
	if (!name)
		name = path;
	if (*path == '\0')
		return (connect_to_server_direct(server_name, port, nick, server_index));
	oper_command = 0;
	write_des[0] = -1;
	write_des[1] = -1;
	if (pipe(write_des) || pipe(read_des))
	{
		if (write_des[0] != -1)
		{
			new_close(write_des[0]);
			new_close(write_des[1]);
		}
		say("Couldn't start new process: %s", strerror(errno));
		return (connect_to_server_direct(server_name, port, nick, server_index));
	}
	switch (pid = fork())
	{
	case -1:
		say("Couldn't start new process: %s\n", strerror(errno));
		return (-1);
	case 0:
		(void) MY_SIGNAL(SIGINT, (sigfunc *)SIG_IGN, 0);
		dup2(read_des[1], 1);
		dup2(write_des[0], 0);
		new_close(read_des[0]);
		new_close(read_des[1]);
		new_close(write_des[0]);
		new_close(write_des[1]);
		snprintf(CP(buffer), sizeof buffer, "%u", port);
		setuid(getuid());
		execl(CP(path), CP(name), server_name, buffer, (u_char *) 0);
		printf("-5 0\n"); /* -1 - -4 returned by connect_by_number() */
		fflush(stdout);
		_exit(1);
	default:
		new_close(read_des[1]);
		new_close(write_des[0]);
		break;
	}
	old_timeout = dgets_timeout(3);
	c = dgets(buffer, sizeof buffer, read_des[0], (u_char *) 0);
	(void) dgets_timeout(old_timeout);
	if ((c == 0) || ((c = my_atoi(buffer)) != 0))
	{
		if (c == -5)
			return (connect_to_server_direct(server_name, port, nick, server_index));
		else
		{
			u_char *ptr;

			if ((ptr = my_index(buffer, ' ')) != NULL)
			{
				ptr++;
				if (my_atoi(ptr) > 0)
		say("Unable to connect to port %d of server %s: %s",
			port, server_name, strerror(my_atoi(ptr)));
				else
		say("Unable to connect to port %d of server %s: Unknown host",
							port, server_name);
			}
			else
		say("Unable to connect to port %d of server %s: Unknown host",
							port, server_name);
			if (is_server_open(from_server))
				say("Connection to server %s resumed...",
						server_list[from_server].name);
			new_close(read_des[0]);
			new_close(write_des[1]);
			return (-1);
		}
	}
	update_all_status();
	add_to_server_list(server_name, port, (u_char *) 0, nick, -1, get_server_version(from_server), SL_ADD_OVERWRITE);
	server_list[from_server].read = read_des[0];
	server_list[from_server].write = write_des[1];
	server_list[from_server].pid = pid;
	server_list[from_server].operator = 0;
	return (0);
#endif /* _Windows */
}
