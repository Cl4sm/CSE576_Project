rte_better(rte *new, rte *old)
{
  int (*better)(rte *, rte *);

  if (!rte_is_valid(old))
    return 1;
  if (!rte_is_valid(new))
    return 0;

  if (new->pref > old->pref)
    return 1;
  if (new->pref < old->pref)
    return 0;
  if (new->attrs->src->proto->proto != old->attrs->src->proto->proto)
    {
      /*
       *  If the user has configured protocol preferences, so that two different protocols
       *  have the same preference, try to break the tie by comparing addresses. Not too
       *  useful, but keeps the ordering of routes unambiguous.
       */
      return new->attrs->src->proto->proto > old->attrs->src->proto->proto;
    }
  if (better = new->attrs->src->proto->rte_better)
    return better(new, old);
  return 0;
}
