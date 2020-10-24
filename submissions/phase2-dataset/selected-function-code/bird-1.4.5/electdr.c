static struct ospf_neighbor *
electdr(list nl)
{
  struct ospf_neighbor *neigh, *n;
  u32 nid;

  n = NULL;
  WALK_LIST(neigh, nl)			/* And now DR */
  {
#ifdef OSPFv2
    nid = ipa_to_u32(neigh->ip);
#else /* OSPFv3 */
    nid = neigh->rid;
#endif

    if (neigh->state >= NEIGHBOR_2WAY)	/* Higher than 2WAY */
      if (neigh->priority > 0)		/* Eligible */
	if (neigh->dr == nid)		/* And declaring itself DR */
	{
	  if (n != NULL)
	  {
	    if (neigh->priority > n->priority)
	      n = neigh;
	    else if (neigh->priority == n->priority)
	      if (neigh->rid > n->rid)
		n = neigh;
	  }
	  else
	  {
	    n = neigh;
	  }
	}
  }

  return (n);
}
