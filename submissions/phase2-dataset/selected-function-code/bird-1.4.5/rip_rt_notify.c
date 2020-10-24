static void
rip_rt_notify(struct proto *p, struct rtable *table UNUSED, struct network *net,
	      struct rte *new, struct rte *old UNUSED, struct ea_list *attrs)
{
  CHK_MAGIC;
  struct rip_entry *e;

  e = fib_find( &P->rtable, &net->n.prefix, net->n.pxlen );
  if (e)
    fib_delete( &P->rtable, e );

  if (new) {
    e = fib_get( &P->rtable, &net->n.prefix, net->n.pxlen );

    e->nexthop = new->attrs->gw;
    e->metric = 0;
    e->whotoldme = IPA_NONE;
    new->u.rip.entry = e;

    e->tag = ea_get_int(attrs, EA_RIP_TAG, 0);
    e->metric = ea_get_int(attrs, EA_RIP_METRIC, 1);
    if (e->metric > P_CF->infinity)
      e->metric = P_CF->infinity;

    if (new->attrs->src->proto == p)
      e->whotoldme = new->attrs->from;

    if (!e->metric)	/* That's okay: this way user can set his own value for external
			   routes in rip. */
      e->metric = 5;
    e->updated = e->changed = now;
    e->flags = 0;
  }
}
