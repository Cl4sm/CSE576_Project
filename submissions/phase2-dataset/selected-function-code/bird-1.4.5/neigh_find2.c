neighbor *
neigh_find2(struct proto *p, ip_addr *a, struct iface *ifa, unsigned flags)
{
  neighbor *n;
  int class, scope = -1;
  unsigned int h = neigh_hash(p, a);
  struct iface *i;
  struct ifa *addr;

  WALK_LIST(n, neigh_hash_table[h])	/* Search the cache */
    if (n->proto == p && ipa_equal(*a, n->addr) && (!ifa || (ifa == n->iface)))
      return n;

  class = ipa_classify(*a);
  if (class < 0)			/* Invalid address */
    return NULL;
  if (((class & IADDR_SCOPE_MASK) == SCOPE_HOST) ||
      (((class & IADDR_SCOPE_MASK) == SCOPE_LINK) && (ifa == NULL)) ||
      !(class & IADDR_HOST))
    return NULL;			/* Bad scope or a somecast */

  if (ifa)
    {
      scope = if_connected(a, ifa, &addr);
      flags |= NEF_BIND;

      if ((scope < 0) && (flags & NEF_ONLINK))
	scope = class & IADDR_SCOPE_MASK;
    }
  else
    WALK_LIST(i, iface_list)
      if ((scope = if_connected(a, i, &addr)) >= 0)
	{
	  ifa = i;
	  break;
	}

  /* scope < 0 means i don't know neighbor */
  /* scope >= 0 implies ifa != NULL */

  if ((scope < 0) && !(flags & NEF_STICKY))
    return NULL;

  n = sl_alloc(neigh_slab);
  n->addr = *a;
  if (scope >= 0)
    {
      add_tail(&neigh_hash_table[h], &n->n);
      add_tail(&ifa->neighbors, &n->if_n);
    }
  else
    {
      add_tail(&sticky_neigh_list, &n->n);
      scope = -1;
    }
  n->iface = ifa;
  n->ifa = addr;
  n->proto = p;
  n->data = NULL;
  n->aux = 0;
  n->flags = flags;
  n->scope = scope;
  return n;
}
