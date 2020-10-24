find_surrogate_fwaddr(struct ospf_area *oa)
{
  struct proto_ospf *po = oa->po;
  struct ospf_iface *ifa;
  struct ifa *a, *cur_addr = NULL;
  int np, cur_np = 0;

  WALK_LIST(ifa, po->iface_list)
  {
    if ((ifa->oa != oa) ||
	(ifa->type == OSPF_IT_VLINK))
      continue;

#ifdef OSPFv2
    a = ifa->addr;
    if (a->flags & IA_PEER)
      continue;

    np = ((a->flags & IA_HOST) || ifa->stub) ? 2 : 1;
    if (np > cur_np)
    {
      cur_addr = a;
      cur_np = np;
    }

#else /* OSPFv3 */
    WALK_LIST(a, ifa->iface->addrs)
    {
      if ((a->flags & IA_SECONDARY) ||
	  (a->flags & IA_PEER) ||
	  (a->scope <= SCOPE_LINK))
	continue;

      np = ((a->flags & IA_HOST) || ifa->stub) ? 2 : 1;
      if (np > cur_np)
      {
	cur_addr = a;
	cur_np = np;
      }
    }
#endif
  }

  return cur_addr ? cur_addr->ip : IPA_NONE;
}
