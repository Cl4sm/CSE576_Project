static void
static_add(struct proto *p, struct static_config *cf, struct static_route *r)
{
  DBG("static_add(%I/%d,%d)\n", r->net, r->masklen, r->dest);
  switch (r->dest)
    {
    case RTD_ROUTER:
      {
	struct neighbor *n = neigh_find2(p, &r->via, r->via_if, NEF_STICKY);
	if (n)
	  {
	    r->chain = n->data;
	    n->data = r;
	    r->neigh = n;
	    if (static_decide(cf, r))
	      static_install(p, r, n->iface);
	    else
	      static_remove(p, r);
	  }
	else
	  {
	    log(L_ERR "Static route destination %I is invalid. Ignoring.", r->via);
	    static_remove(p, r);
	  }
	break;
      }

    case RTD_DEVICE:
      break;

    case RTD_MULTIPATH:
      {
	int count = 0;
	struct static_route *r2;

	for (r2 = r->mp_next; r2; r2 = r2->mp_next)
	  {
	    struct neighbor *n = neigh_find2(p, &r2->via, r2->via_if, NEF_STICKY);
	    if (n)
	      {
		r2->chain = n->data;
		n->data = r2;
		r2->neigh = n;
		r2->installed = static_decide(cf, r2);
		count += r2->installed;
	      }
	    else
	      {
		log(L_ERR "Static route destination %I is invalid. Ignoring.", r2->via);
		r2->installed = 0;
	      }
	  }

	if (count)
	  static_install(p, r, NULL);
	else
	  static_remove(p, r);
	break;
      }

    default:
      static_install(p, r, NULL);
    }
}
