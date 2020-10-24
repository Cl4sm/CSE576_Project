static int
bgp_community_filter(struct bgp_proto *p, rte *e)
{
  eattr *a;
  struct adata *d;

  /* Check if we aren't forbidden to export the route by communities */
  a = ea_find(e->attrs->eattrs, EA_CODE(EAP_BGP, BA_COMMUNITY));
  if (a)
    {
      d = a->u.ptr;
      if (int_set_contains(d, BGP_COMM_NO_ADVERTISE))
	{
	  DBG("\tNO_ADVERTISE\n");
	  return 1;
	}
      if (!p->is_internal &&
	  (int_set_contains(d, BGP_COMM_NO_EXPORT) ||
	   int_set_contains(d, BGP_COMM_NO_EXPORT_SUBCONFED)))
	{
	  DBG("\tNO_EXPORT\n");
	  return 1;
	}
    }

  return 0;
}
