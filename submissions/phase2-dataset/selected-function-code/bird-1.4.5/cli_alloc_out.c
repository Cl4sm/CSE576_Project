cli_alloc_out(cli *c, int size)
{
  struct cli_out *o;

  if (!(o = c->tx_write) || o->wpos + size > o->end)
    {
      if (!o && c->tx_buf)
	o = c->tx_buf;
      else
	{
	  o = mb_alloc(c->pool, sizeof(struct cli_out) + CLI_TX_BUF_SIZE);
	  if (c->tx_write)
	    c->tx_write->next = o;
	  else
	    c->tx_buf = o;
	  o->wpos = o->outpos = o->buf;
	  o->end = o->buf + CLI_TX_BUF_SIZE;
	}
      c->tx_write = o;
      if (!c->tx_pos)
	c->tx_pos = o;
      o->next = NULL;
    }
  o->wpos += size;
  return o->wpos - size;
}
