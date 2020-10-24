static int
bgp_update_attrs(struct bgp_proto *p, rte *e, ea_list **attrs, struct linpool *pool, int rr)
{
  eattr *a;

  if (!p->is_internal && !p->rs_client)
    {
      bgp_path_prepend(e, attrs, pool, p->local_as);

      /* The MULTI_EXIT_DISC attribute received from a neighboring AS MUST NOT be
       * propagated to other neighboring ASes.
       * Perhaps it would be better to undefine it.
       */
      a = ea_find(e->attrs->eattrs, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC));
      if (a)
	bgp_attach_attr(attrs, pool, BA_MULTI_EXIT_DISC, 0);
    }

  /* iBGP -> keep next_hop, eBGP multi-hop -> use source_addr,
   * eBGP single-hop -> keep next_hop if on the same iface.
   * If the next_hop is zero (i.e. link-local), keep only if on the same iface.
   *
   * Note that same-iface-check uses iface from route, which is based on gw.
   */
  a = ea_find(e->attrs->eattrs, EA_CODE(EAP_BGP, BA_NEXT_HOP));
  if (a && !p->cf->next_hop_self && 
      (p->cf->next_hop_keep ||
       (p->is_internal && ipa_nonzero(*((ip_addr *) a->u.ptr->data))) ||
       (p->neigh && (e->attrs->iface == p->neigh->iface))))
    {
      /* Leave the original next hop attribute, will check later where does it point */
    }
  else
    {
      /* Need to create new one */
      byte *b = bgp_attach_attr_wa(attrs, pool, BA_NEXT_HOP, NEXT_HOP_LENGTH);
      set_next_hop(b, p->source_addr);
    }

  if (rr)
    {
      /* Handling route reflection, RFC 4456 */
      struct bgp_proto *src = (struct bgp_proto *) e->attrs->src->proto;

      a = ea_find(e->attrs->eattrs, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID));
      if (!a)
	bgp_attach_attr(attrs, pool, BA_ORIGINATOR_ID, src->remote_id);

      /* We attach proper cluster ID according to whether the route is entering or leaving the cluster */
      bgp_cluster_list_prepend(e, attrs, pool, src->rr_client ? src->rr_cluster_id : p->rr_cluster_id);

      /* Two RR clients with different cluster ID, hmmm */
      if (src->rr_client && p->rr_client && (src->rr_cluster_id != p->rr_cluster_id))
	bgp_cluster_list_prepend(e, attrs, pool, p->rr_cluster_id);
    }

  return 0;				/* Leave decision to the filters */
}
