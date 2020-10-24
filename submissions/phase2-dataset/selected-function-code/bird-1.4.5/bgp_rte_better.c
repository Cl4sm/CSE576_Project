int
bgp_rte_better(rte *new, rte *old)
{
  struct bgp_proto *new_bgp = (struct bgp_proto *) new->attrs->src->proto;
  struct bgp_proto *old_bgp = (struct bgp_proto *) old->attrs->src->proto;
  eattr *x, *y;
  u32 n, o;

  /* Skip suppressed routes (see bgp_rte_recalculate()) */
  n = new->u.bgp.suppressed;
  o = old->u.bgp.suppressed;
  if (n > o)
    return 0;
  if (n < o)
    return 1;

  /* RFC 4271 9.1.2.1. Route resolvability test */
  n = rte_resolvable(new);
  o = rte_resolvable(old);
  if (n > o)
    return 1;
  if (n < o)
    return 0;

  /* Start with local preferences */
  x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_LOCAL_PREF));
  y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_LOCAL_PREF));
  n = x ? x->u.data : new_bgp->cf->default_local_pref;
  o = y ? y->u.data : old_bgp->cf->default_local_pref;
  if (n > o)
    return 1;
  if (n < o)
    return 0;

  /* RFC 4271 9.1.2.2. a)  Use AS path lengths */
  if (new_bgp->cf->compare_path_lengths || old_bgp->cf->compare_path_lengths)
    {
      x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_AS_PATH));
      y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_AS_PATH));
      n = x ? as_path_getlen(x->u.ptr) : AS_PATH_MAXLEN;
      o = y ? as_path_getlen(y->u.ptr) : AS_PATH_MAXLEN;
      if (n < o)
	return 1;
      if (n > o)
	return 0;
    }

  /* RFC 4271 9.1.2.2. b) Use origins */
  x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_ORIGIN));
  y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_ORIGIN));
  n = x ? x->u.data : ORIGIN_INCOMPLETE;
  o = y ? y->u.data : ORIGIN_INCOMPLETE;
  if (n < o)
    return 1;
  if (n > o)
    return 0;

  /* RFC 4271 9.1.2.2. c) Compare MED's */
  /* Proper RFC 4271 path selection cannot be interpreted as finding
   * the best path in some ordering. It is implemented partially in
   * bgp_rte_recalculate() when deterministic_med option is
   * active. Without that option, the behavior is just an
   * approximation, which in specific situations may lead to
   * persistent routing loops, because it is nondeterministic - it
   * depends on the order in which routes appeared. But it is also the
   * same behavior as used by default in Cisco routers, so it is
   * probably not a big issue.
   */
  if (new_bgp->cf->med_metric || old_bgp->cf->med_metric ||
      (bgp_get_neighbor(new) == bgp_get_neighbor(old)))
    {
      x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC));
      y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_MULTI_EXIT_DISC));
      n = x ? x->u.data : new_bgp->cf->default_med;
      o = y ? y->u.data : old_bgp->cf->default_med;
      if (n < o)
	return 1;
      if (n > o)
	return 0;
    }

  /* RFC 4271 9.1.2.2. d) Prefer external peers */
  if (new_bgp->is_internal > old_bgp->is_internal)
    return 0;
  if (new_bgp->is_internal < old_bgp->is_internal)
    return 1;

  /* RFC 4271 9.1.2.2. e) Compare IGP metrics */
  n = new_bgp->cf->igp_metric ? new->attrs->igp_metric : 0;
  o = old_bgp->cf->igp_metric ? old->attrs->igp_metric : 0;
  if (n < o)
    return 1;
  if (n > o)
    return 0;

  /* RFC 4271 9.1.2.2. f) Compare BGP identifiers */
  /* RFC 4456 9. a) Use ORIGINATOR_ID instead of local neighor ID */
  x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID));
  y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_ORIGINATOR_ID));
  n = x ? x->u.data : new_bgp->remote_id;
  o = y ? y->u.data : old_bgp->remote_id;

  /* RFC 5004 - prefer older routes */
  /* (if both are external and from different peer) */
  if ((new_bgp->cf->prefer_older || old_bgp->cf->prefer_older) &&
      !new_bgp->is_internal && n != o)
    return 0;

  /* rest of RFC 4271 9.1.2.2. f) */
  if (n < o)
    return 1;
  if (n > o)
    return 0;

  /* RFC 4456 9. b) Compare cluster list lengths */
  x = ea_find(new->attrs->eattrs, EA_CODE(EAP_BGP, BA_CLUSTER_LIST));
  y = ea_find(old->attrs->eattrs, EA_CODE(EAP_BGP, BA_CLUSTER_LIST));
  n = x ? int_set_get_size(x->u.ptr) : 0;
  o = y ? int_set_get_size(y->u.ptr) : 0;
  if (n < o)
    return 1;
  if (n > o)
    return 0;

  /* RFC 4271 9.1.2.2. g) Compare peer IP adresses */
  return (ipa_compare(new_bgp->cf->remote_ip, old_bgp->cf->remote_ip) < 0);
}
