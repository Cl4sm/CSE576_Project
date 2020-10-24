static int
ospf_rte_better(struct rte *new, struct rte *old)
{
  if (new->u.ospf.metric1 == LSINFINITY)
    return 0;

  if(new->attrs->source < old->attrs->source) return 1;
  if(new->attrs->source > old->attrs->source) return 0;

  if(new->attrs->source == RTS_OSPF_EXT2)
  {
    if(new->u.ospf.metric2 < old->u.ospf.metric2) return 1;
    if(new->u.ospf.metric2 > old->u.ospf.metric2) return 0;
  }

  if (new->u.ospf.metric1 < old->u.ospf.metric1)
    return 1;

  return 0;			/* Old is shorter or same */
}
