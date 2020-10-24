server_read(void)
{
  int c;
  byte *start, *p;

 redo:
  c = read(server_fd, server_read_pos, server_read_buf + sizeof(server_read_buf) - server_read_pos);
  if (!c)
    die("Connection closed by server.");
  if (c < 0)
    {
      if (errno == EINTR)
	goto redo;
      else
	die("Server read error: %m");
    }

  start = server_read_buf;
  p = server_read_pos;
  server_read_pos += c;
  while (p < server_read_pos)
    if (*p++ == '\n')
      {
	p[-1] = 0;
	server_got_reply(start);
	start = p;
      }
  if (start != server_read_buf)
    {
      int l = server_read_pos - start;
      memmove(server_read_buf, start, l);
      server_read_pos = server_read_buf + l;
    }
  else if (server_read_pos == server_read_buf + sizeof(server_read_buf))
    {
      strcpy(server_read_buf, "?<too-long>");
      server_read_pos = server_read_buf + 11;
    }
}
