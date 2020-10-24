if_connected(ip_addr *a, struct iface *i, struct ifa **ap)
{
  struct ifa *b;

  if (!(i->flags & IF_UP))
  {
    *ap = NULL;
    return -1;
  }

  WALK_LIST(b, i->addrs)
    {
      *ap = b;

      if (ipa_equal(*a, b->ip))
	return SCOPE_HOST;
      if (b->flags & IA_PEER)
	{
	  if (ipa_equal(*a, b->opposite))
	    return b->scope;
	}
      else
	{
	  if (ipa_in_net(*a, b->prefix, b->pxlen))
	    {
#ifndef IPV6
	      if ((b->pxlen < (BITS_PER_IP_ADDRESS - 1)) &&
		  (ipa_equal(*a, b->prefix) ||	/* Network address */
		   ipa_equal(*a, b->brd)))	/* Broadcast */
	      {
		*ap = NULL;
		return -1;
	      }
#endif

	      return b->scope;
	    }
	}
      }

  *ap = NULL;
  return -1;
}
