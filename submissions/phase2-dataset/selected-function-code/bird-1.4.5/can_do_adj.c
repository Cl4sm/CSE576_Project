can_do_adj(struct ospf_neighbor *n)
{
  struct ospf_iface *ifa;
  struct proto *p;
  int i;

  ifa = n->ifa;
  p = (struct proto *) (ifa->oa->po);
  i = 0;

  switch (ifa->type)
  {
  case OSPF_IT_PTP:
  case OSPF_IT_PTMP:
  case OSPF_IT_VLINK:
    i = 1;
    break;
  case OSPF_IT_BCAST:
  case OSPF_IT_NBMA:
    switch (ifa->state)
    {
    case OSPF_IS_DOWN:
    case OSPF_IS_LOOP:
      bug("%s: Iface %s in down state?", p->name, ifa->ifname);
      break;
    case OSPF_IS_WAITING:
      DBG("%s: Neighbor? on iface %s\n", p->name, ifa->ifname);
      break;
    case OSPF_IS_DROTHER:
      if (((n->rid == ifa->drid) || (n->rid == ifa->bdrid))
	  && (n->state >= NEIGHBOR_2WAY))
	i = 1;
      break;
    case OSPF_IS_PTP:
    case OSPF_IS_BACKUP:
    case OSPF_IS_DR:
      if (n->state >= NEIGHBOR_2WAY)
	i = 1;
      break;
    default:
      bug("%s: Iface %s in unknown state?", p->name, ifa->ifname);
      break;
    }
    break;
  default:
    bug("%s: Iface %s is unknown type?", p->name, ifa->ifname);
    break;
  }
  DBG("%s: Iface %s can_do_adj=%d\n", p->name, ifa->ifname, i);
  return i;
}
