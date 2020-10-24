ospf_neigh_sm(struct ospf_neighbor *n, int event)
{
  struct proto_ospf *po = n->ifa->oa->po;
  struct proto *p = &po->proto;

  DBG("Neighbor state machine for neighbor %I, event '%s'\n", n->ip,
	     ospf_inm[event]);

  switch (event)
  {
  case INM_START:
    neigh_chstate(n, NEIGHBOR_ATTEMPT);
    /* NBMA are used different way */
    break;
  case INM_HELLOREC:
    switch (n->state)
    {
    case NEIGHBOR_ATTEMPT:
    case NEIGHBOR_DOWN:
      neigh_chstate(n, NEIGHBOR_INIT);
    default:
      tm_start(n->inactim, n->ifa->deadint);	/* Restart inactivity timer */
      break;
    }
    break;
  case INM_2WAYREC:
    if (n->state < NEIGHBOR_2WAY)
      neigh_chstate(n, NEIGHBOR_2WAY);
    if ((n->state == NEIGHBOR_2WAY) && can_do_adj(n))
      neigh_chstate(n, NEIGHBOR_EXSTART);
    break;
  case INM_NEGDONE:
    if (n->state == NEIGHBOR_EXSTART)
    {
      neigh_chstate(n, NEIGHBOR_EXCHANGE);

      /* Reset DB summary list iterator */
      s_get(&(n->dbsi));
      s_init(&(n->dbsi), &po->lsal);

      while (!EMPTY_LIST(n->ackl[ACKL_DELAY]))
      {
	struct lsah_n *no;
	no = (struct lsah_n *) HEAD(n->ackl[ACKL_DELAY]);
	rem_node(NODE no);
	mb_free(no);
      }
    }
    else
      bug("NEGDONE and I'm not in EXSTART?");
    break;
  case INM_EXDONE:
    neigh_chstate(n, NEIGHBOR_LOADING);
    break;
  case INM_LOADDONE:
    neigh_chstate(n, NEIGHBOR_FULL);
    break;
  case INM_ADJOK:
    switch (n->state)
    {
    case NEIGHBOR_2WAY:
      /* Can In build adjacency? */
      if (can_do_adj(n))
      {
	neigh_chstate(n, NEIGHBOR_EXSTART);
      }
      break;
    default:
      if (n->state >= NEIGHBOR_EXSTART)
	if (!can_do_adj(n))
	{
	  reset_lists(n);
	  neigh_chstate(n, NEIGHBOR_2WAY);
	}
      break;
    }
    break;
  case INM_SEQMIS:
  case INM_BADLSREQ:
    if (n->state >= NEIGHBOR_EXCHANGE)
    {
      reset_lists(n);
      neigh_chstate(n, NEIGHBOR_EXSTART);
    }
    break;
  case INM_KILLNBR:
  case INM_LLDOWN:
  case INM_INACTTIM:
    reset_lists(n);
    neigh_chstate(n, NEIGHBOR_DOWN);
    break;
  case INM_1WAYREC:
    reset_lists(n);
    neigh_chstate(n, NEIGHBOR_INIT);
    break;
  default:
    bug("%s: INM - Unknown event?", p->name);
    break;
  }
}
