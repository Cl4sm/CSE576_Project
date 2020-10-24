roa_show_cont(struct cli *c)
{
  struct roa_show_data *d = c->rover;
  struct fib *fib = &d->table->fib;
  struct fib_iterator *it = &d->fit;
  struct roa_node *rn;
  unsigned max = 32;

  FIB_ITERATE_START(fib, it, f)
    {
      rn = (struct roa_node *) f;

      if (!max--)
	{
	  FIB_ITERATE_PUT(it, f);
	  return;
	}

      if ((d->mode == ROA_SHOW_ALL) ||
	  net_in_net(rn->n.prefix, rn->n.pxlen, d->prefix, d->pxlen))
	roa_show_node(c, rn, 0, d->asn);
    }
  FIB_ITERATE_END(f);

  cli_printf(c, 0, "");
  c->cont = c->cleanup = NULL;
}
