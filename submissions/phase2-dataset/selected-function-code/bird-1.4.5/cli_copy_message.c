cli_copy_message(cli *c)
{
  byte *p, *q;
  unsigned int cnt = 2;

  if (c->ring_overflow)
    {
      byte buf[64];
      int n = bsprintf(buf, "<%d messages lost>\n", c->ring_overflow);
      c->ring_overflow = 0;
      memcpy(cli_alloc_out(c, n), buf, n);
    }
  p = c->ring_read;
  while (*p)
    {
      cnt++;
      p++;
      if (p == c->ring_end)
	p = c->ring_buf;
      ASSERT(p != c->ring_write);
    }
  c->async_msg_size += cnt;
  q = cli_alloc_out(c, cnt);
  *q++ = '+';
  p = c->ring_read;
  do
    {
      *q = *p++;
      if (p == c->ring_end)
	p = c->ring_buf;
    }
  while (*q++);
  c->ring_read = p;
  q[-1] = '\n';
}
