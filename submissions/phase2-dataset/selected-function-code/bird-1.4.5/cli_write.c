static void
cli_write(cli *c)
{
  sock *s = c->priv;

  while (c->tx_pos)
    {
      struct cli_out *o = c->tx_pos;

      int len = o->wpos - o->outpos;
      s->tbuf = o->outpos;
      o->outpos = o->wpos;

      if (sk_send(s, len) <= 0)
	return;

      c->tx_pos = o->next;
    }

  /* Everything is written */
  s->tbuf = NULL;
  cli_written(c);
}
