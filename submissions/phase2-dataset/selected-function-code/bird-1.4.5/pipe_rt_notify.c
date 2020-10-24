pipe_rt_notify(struct proto *P, rtable *src_table, net *n, rte *new, rte *old, ea_list *attrs)
{
  struct pipe_proto *p = (struct pipe_proto *) P;
  struct announce_hook *ah = (src_table == P->table) ? p->peer_ahook : P->main_ahook;
  rtable *dst_table = ah->table;
  struct rte_src *src;

  net *nn;
  rte *e;
  rta a;

  if (!new && !old)
    return;

  if (dst_table->pipe_busy)
    {
      log(L_ERR "Pipe loop detected when sending %I/%d to table %s",
	  n->n.prefix, n->n.pxlen, dst_table->name);
      return;
    }

  nn = net_get(dst_table, n->n.prefix, n->n.pxlen);
  if (new)
    {
      memcpy(&a, new->attrs, sizeof(rta));

      if (p->mode == PIPE_OPAQUE)
	{
	  a.src = P->main_source;
	  a.source = RTS_PIPE;
	}

      a.aflags = 0;
      a.eattrs = attrs;
      a.hostentry = NULL;
      e = rte_get_temp(&a);
      e->net = nn;
      e->pflags = 0;

      if (p->mode == PIPE_TRANSPARENT)
	{
	  /* Copy protocol specific embedded attributes. */
	  memcpy(&(e->u), &(new->u), sizeof(e->u));
	  e->pref = new->pref;
	  e->pflags = new->pflags;
	}

      src = a.src;
    }
  else
    {
      e = NULL;
      src = old->attrs->src;
    }

  src_table->pipe_busy = 1;
  rte_update2(ah, nn, e, src);
  src_table->pipe_busy = 0;
}
