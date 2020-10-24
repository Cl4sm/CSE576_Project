cli_get_command(cli *c)
{
  sock *s = c->priv;
  byte *t = c->rx_aux ? : s->rbuf;
  byte *tend = s->rpos;
  byte *d = c->rx_pos;
  byte *dend = c->rx_buf + CLI_RX_BUF_SIZE - 2;

  while (t < tend)
    {
      if (*t == '\r')
	t++;
      else if (*t == '\n')
	{
	  t++;
	  c->rx_pos = c->rx_buf;
	  c->rx_aux = t;
	  *d = 0;
	  return (d < dend) ? 1 : -1;
	}
      else if (d < dend)
	*d++ = *t++;
    }
  c->rx_aux = s->rpos = s->rbuf;
  c->rx_pos = d;
  return 0;
}
