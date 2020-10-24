static void
bgp_neigh_notify(neighbor *n)
{
  struct bgp_proto *p = (struct bgp_proto *) n->proto;

  if (! (n->flags & NEF_STICKY))
    return;

  if (n->scope > 0)
    {
      if ((p->p.proto_state == PS_START) && (p->start_state == BSS_PREPARE))
	{
	  BGP_TRACE(D_EVENTS, "Neighbor found");
	  bgp_start_neighbor(p);
	}
    }
  else
    {
      if ((p->p.proto_state == PS_START) || (p->p.proto_state == PS_UP))
	{
	  BGP_TRACE(D_EVENTS, "Neighbor lost");
	  bgp_store_error(p, NULL, BE_MISC, BEM_NEIGHBOR_LOST);
	  bgp_stop(p, 0);
	}
    }
}
