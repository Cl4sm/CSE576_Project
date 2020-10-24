electbdr(list nl)
{
  struct ospf_neighbor *neigh, *n1, *n2;
  u32 nid;

  n1 = NULL;
  n2 = NULL;
  WALK_LIST(neigh, nl)			/* First try those decl. themselves */
  {
#ifdef OSPFv2
    nid = ipa_to_u32(neigh->ip);
#else /* OSPFv3 */
    nid = neigh->rid;
#endif

    if (neigh->state >= NEIGHBOR_2WAY)	/* Higher than 2WAY */
      if (neigh->priority > 0)		/* Eligible */
	if (neigh->dr != nid)		/* And not decl. itself DR */
	{
	  if (neigh->bdr == nid)	/* Declaring BDR */
	  {
	    if (n1 != NULL)
	    {
	      if (neigh->priority > n1->priority)
		n1 = neigh;
	      else if (neigh->priority == n1->priority)
		if (neigh->rid > n1->rid)
		  n1 = neigh;
	    }
	    else
	    {
	      n1 = neigh;
	    }
	  }
	  else			/* And NOT declaring BDR */
	  {
	    if (n2 != NULL)
	    {
	      if (neigh->priority > n2->priority)
		n2 = neigh;
	      else if (neigh->priority == n2->priority)
		if (neigh->rid > n2->rid)
		  n2 = neigh;
	    }
	    else
	    {
	      n2 = neigh;
	    }
	  }
	}
  }
  if (n1 == NULL)
    n1 = n2;

  return (n1);
}
