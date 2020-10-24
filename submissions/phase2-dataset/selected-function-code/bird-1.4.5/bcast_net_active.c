bcast_net_active(struct ospf_iface *ifa)
{
  struct ospf_neighbor *neigh;

  if (ifa->state == OSPF_IS_WAITING)
    return 0;

  WALK_LIST(neigh, ifa->neigh_list)
    {
      if (neigh->state == NEIGHBOR_FULL)
	{
	  if (neigh->rid == ifa->drid)
	    return 1;

	  if (ifa->state == OSPF_IS_DR)
	    return 1;
	}
    }

  return 0;
}
