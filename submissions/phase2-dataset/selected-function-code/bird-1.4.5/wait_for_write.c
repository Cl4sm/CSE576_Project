wait_for_write(int fd)
{
  while (1)
    {
      int rv;
      fd_set set;
      FD_ZERO(&set);
      FD_SET(fd, &set);

      rv = select(fd+1, NULL, &set, NULL, NULL);
      if (rv < 0)
	{
	  if (errno == EINTR)
	    continue;
	  else
	    die("select: %m");
	}

      if (FD_ISSET(server_fd, &set))
	return;
    }
}
