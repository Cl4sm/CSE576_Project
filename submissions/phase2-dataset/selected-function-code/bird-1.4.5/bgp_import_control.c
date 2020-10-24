int
bgp_import_control(struct proto *P, rte **new, ea_list **attrs, struct linpool *pool)
{
  rte *e = *new;
  struct bgp_proto *p = (struct bgp_proto *) P;
  struct bgp_proto *new_bgp = (e->attrs->src->proto->proto == &proto_bgp) ?
    (struct bgp_proto *) e->attrs->src->proto : NULL;

  if (p == new_bgp)			/* Poison reverse updates */
    return -1;
  if (new_bgp)
    {
      /* We should check here for cluster list loop, because the receiving BGP instance
	 might have different cluster ID  */
      if (bgp_cluster_list_loopy(p, e->attrs))
	return -1;

      if (p->cf->interpret_communities && bgp_community_filter(p, e))
	return -1;

      if (p->local_as == new_bgp->local_as && p->is_internal && new_bgp->is_internal)
	{
	  /* Redistribution of internal routes with IBGP */
	  if (p->rr_client || new_bgp->rr_client)
	    /* Route reflection, RFC 4456 */
	    return bgp_update_attrs(p, e, attrs, pool, 1);
	  else
	    return -1;
	}
      else
	return bgp_update_attrs(p, e, attrs, pool, 0);
    }
  else
    return bgp_create_attrs(p, e, attrs, pool);
}
