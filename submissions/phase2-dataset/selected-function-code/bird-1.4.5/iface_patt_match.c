int
iface_patt_match(struct iface_patt *ifp, struct iface *i, struct ifa *a)
{
  struct iface_patt_node *p;

  WALK_LIST(p, ifp->ipn_list)
    {
      char *t = p->pattern;
      int pos = p->positive;

      if (t)
	{
	  if (*t == '-')
	    {
	      t++;
	      pos = !pos;
	    }

	  if (!patmatch(t, i->name))
	    continue;
	}

      if (p->pxlen == 0)
	return pos;

      if (!a)
	continue;

      if (ipa_in_net(a->ip, p->prefix, p->pxlen))
	return pos;

      if ((a->flags & IA_PEER) &&
	  ipa_in_net(a->opposite, p->prefix, p->pxlen))
	return pos;
	  
      continue;
    }

  return 0;
}
