static void
server_send(char *cmd)
{
  int l = strlen(cmd);
  byte *z = alloca(l + 1);

  memcpy(z, cmd, l);
  z[l++] = '\n';
  while (l)
    {
      int cnt = write(server_fd, z, l);

      if (cnt < 0)
	{
	  if (errno == EAGAIN)
	    wait_for_write(server_fd);
	  else if (errno == EINTR)
	    continue;
	  else
	    die("Server write error: %m");
	}
      else
	{
	  l -= cnt;
	  z += cnt;
	}
    }
}
