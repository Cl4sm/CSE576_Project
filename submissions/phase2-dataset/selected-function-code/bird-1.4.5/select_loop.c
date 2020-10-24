static void
select_loop(void)
{
  int rv;
  while (1)
    {
      if (init && !busy)
	init_commands();

      if (!init)
	input_notify(!busy);

      fd_set select_fds;
      FD_ZERO(&select_fds);

      FD_SET(server_fd, &select_fds);
      if (!busy)
	FD_SET(0, &select_fds);

      rv = select(server_fd+1, &select_fds, NULL, NULL, NULL);
      if (rv < 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    die("select: %m");
	}

      if (FD_ISSET(0, &select_fds))
	{
	  input_read();
	  continue;
	}

      if (FD_ISSET(server_fd, &select_fds))
	{
	  server_read();
	  continue;
	}
    }
}
