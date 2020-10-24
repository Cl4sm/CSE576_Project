static void
bgp_remove_as4_attrs(struct bgp_proto *p, rta *a)
{
  unsigned id1 = EA_CODE(EAP_BGP, BA_AS4_PATH);
  unsigned id2 = EA_CODE(EAP_BGP, BA_AS4_AGGREGATOR);
  ea_list **el = &(a->eattrs);

  /* We know that ea_lists constructed in bgp_decode attrs have one attribute per ea_list struct */
  while (*el != NULL)
    {
      unsigned fid = (*el)->attrs[0].id;

      if ((fid == id1) || (fid == id2))
	{
	  *el = (*el)->next;
	  if (p->as4_session)
	    log(L_WARN "%s: Unexpected AS4_* attributes received", p->p.name);
	}
      else
	el = &((*el)->next);
    }
}
