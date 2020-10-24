rt_update_hostentry(rtable *tab, struct hostentry *he)
{
  rta *old_src = he->src;
  int pxlen = 0;

  /* Reset the hostentry */ 
  he->src = NULL;
  he->gw = IPA_NONE;
  he->dest = RTD_UNREACHABLE;
  he->igp_metric = 0;

  net *n = net_route(tab, he->addr, MAX_PREFIX_LENGTH);
  if (n)
    {
      rte *e = n->routes;
      rta *a = e->attrs;
      pxlen = n->n.pxlen;

      if (a->hostentry)
	{
	  /* Recursive route should not depend on another recursive route */
	  log(L_WARN "Next hop address %I resolvable through recursive route for %I/%d",
	      he->addr, n->n.prefix, pxlen);
	  goto done;
	}

      if (a->dest == RTD_DEVICE)
	{
	  if (if_local_addr(he->addr, a->iface))
	    {
	      /* The host address is a local address, this is not valid */
	      log(L_WARN "Next hop address %I is a local address of iface %s",
		  he->addr, a->iface->name);
	      goto done;
      	    }

	  /* The host is directly reachable, use link as a gateway */
	  he->gw = he->link;
	  he->dest = RTD_ROUTER;
	}
      else
	{
	  /* The host is reachable through some route entry */
	  he->gw = a->gw;
	  he->dest = a->dest;
	}

      he->src = rta_clone(a);
      he->igp_metric = rt_get_igp_metric(e);
    }

 done:
  /* Add a prefix range to the trie */
  trie_add_prefix(tab->hostcache->trie, he->addr, MAX_PREFIX_LENGTH, pxlen, MAX_PREFIX_LENGTH);

  rta_free(old_src);
  return old_src != he->src;
}
