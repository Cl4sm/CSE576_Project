void
cli_echo(unsigned int class, byte *msg)
{
  unsigned len, free, i, l;
  cli *c;
  byte *m;

  if (!cli_log_inited || EMPTY_LIST(cli_log_hooks))
    return;
  len = strlen(msg) + 1;
  WALK_LIST(c, cli_log_hooks)
    {
      if (!(c->log_mask & (1 << class)))
	continue;
      if (c->ring_read <= c->ring_write)
	free = (c->ring_end - c->ring_buf) - (c->ring_write - c->ring_read + 1);
      else
	free = c->ring_read - c->ring_write - 1;
      if ((len > free) ||
	  (free < c->log_threshold && class < (unsigned) L_INFO[0]))
	{
	  c->ring_overflow++;
	  continue;
	}
      if (c->ring_read == c->ring_write)
	ev_schedule(c->event);
      m = msg;
      l = len;
      while (l)
	{
	  if (c->ring_read <= c->ring_write)
	    i = c->ring_end - c->ring_write;
	  else
	    i = c->ring_read - c->ring_write;
	  if (i > l)
	    i = l;
	  memcpy(c->ring_write, m, i);
	  m += i;
	  l -= i;
	  c->ring_write += i;
	  if (c->ring_write == c->ring_end)
	    c->ring_write = c->ring_buf;
	}
    }
}
