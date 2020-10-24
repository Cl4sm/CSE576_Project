static void
bgp_reconstruct_4b_atts(struct bgp_proto *p, rta *a, struct linpool *pool)
{
  eattr *p2 =ea_find(a->eattrs, EA_CODE(EAP_BGP, BA_AS_PATH));
  eattr *p4 =ea_find(a->eattrs, EA_CODE(EAP_BGP, BA_AS4_PATH));
  eattr *a2 =ea_find(a->eattrs, EA_CODE(EAP_BGP, BA_AGGREGATOR));
  eattr *a4 =ea_find(a->eattrs, EA_CODE(EAP_BGP, BA_AS4_AGGREGATOR));
  int a4_removed = 0;

  if (a4 && !as4_aggregator_valid(a4->u.ptr))
    {
      log(L_WARN "%s: AS4_AGGREGATOR attribute is invalid, skipping attribute", p->p.name);
      a4 = NULL;
      a4_removed = 1;
    }

  if (a2)
    {
      u32 a2_as = get_u16(a2->u.ptr->data);

      if (a4)
	{
	  if (a2_as != AS_TRANS)
	    {
	      /* Routes were aggregated by old router and therefore AS4_PATH
	       * and AS4_AGGREGATOR is invalid
	       *
	       * Convert AS_PATH and AGGREGATOR to 4B format and finish.
	       */

	      a2->u.ptr = bgp_aggregator_convert_to_new(a2->u.ptr, pool);
	      p2->u.ptr = bgp_merge_as_paths(p2->u.ptr, NULL, AS_PATH_MAXLEN, pool);

	      return;
	    }
	  else
	    {
	      /* Common case, use AS4_AGGREGATOR attribute */
	      a2->u.ptr = a4->u.ptr;
	    }
	}
      else
	{
	  /* Common case, use old AGGREGATOR attribute */
	  a2->u.ptr = bgp_aggregator_convert_to_new(a2->u.ptr, pool);

	  if ((a2_as == AS_TRANS) && !a4_removed)
	    log(L_WARN "%s: AGGREGATOR attribute contain AS_TRANS, but AS4_AGGREGATOR is missing", p->p.name);
	}
    }
  else
    if (a4)
      log(L_WARN "%s: AS4_AGGREGATOR attribute received, but AGGREGATOR attribute is missing", p->p.name);

  int p2_len = as_path_getlen_int(p2->u.ptr, 2);
  int p4_len = p4 ? validate_as4_path(p, p4->u.ptr) : -1;

  if (p4 && (p4_len < 0))
    log(L_WARN "%s: AS4_PATH attribute is malformed, skipping attribute", p->p.name);

  if ((p4_len <= 0) || (p2_len < p4_len))
    p2->u.ptr = bgp_merge_as_paths(p2->u.ptr, NULL, AS_PATH_MAXLEN, pool);
  else
    p2->u.ptr = bgp_merge_as_paths(p2->u.ptr, p4->u.ptr, p2_len - p4_len, pool);
}
