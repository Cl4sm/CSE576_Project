rip_rte_better(struct rte *new, struct rte *old)
{
  struct proto *p = new->attrs->src->proto;

  if (ipa_equal(old->attrs->from, new->attrs->from))
    return 1;

  if (old->u.rip.metric < new->u.rip.metric)
    return 0;

  if (old->u.rip.metric > new->u.rip.metric)
    return 1;

  if (old->attrs->src->proto == new->attrs->src->proto)		/* This does not make much sense for different protocols */
    if ((old->u.rip.metric == new->u.rip.metric) &&
	((now - old->lastmod) > (P_CF->timeout_time / 2)))
      return 1;

  return 0;
}
