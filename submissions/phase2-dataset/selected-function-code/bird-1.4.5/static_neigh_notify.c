static_neigh_notify(struct neighbor *n)
{
  struct proto *p = n->proto;
  struct static_route *r;

  DBG("Static: neighbor notify for %I: iface %p\n", n->addr, n->iface);
  for(r=n->data; r; r=r->chain)
    switch (r->dest)
      {
      case RTD_ROUTER:
	if (static_decide((struct static_config *) p->cf, r))
	  static_install(p, r, n->iface);
	else
	  static_remove(p, r);
	break;

      case RTD_NONE: /* a part of multipath route */
	{
	  int decision = static_decide((struct static_config *) p->cf, r);
	  if (decision == r->installed)
	    break; /* no change */
	  r->installed = decision;

	  struct static_route *r1, *r2;
	  int count = 0;
	  r1 = (void *) r->if_name; /* really */
	  for (r2 = r1->mp_next; r2; r2 = r2->mp_next)
	    count += r2->installed;

	  if (count)
	    {
	      /* Set of nexthops changed - force reinstall */
	      r1->installed = 0;
	      static_install(p, r1, NULL);
	    }
	  else
	    static_remove(p, r1);

	  break;
	}
      }
}
