ospf_neighbor_new(struct ospf_iface *ifa)
{
  struct proto *p = (struct proto *) (ifa->oa->po);
  struct proto_ospf *po = ifa->oa->po;
  struct pool *pool = rp_new(p->pool, "OSPF Neighbor");
  struct ospf_neighbor *n = mb_allocz(pool, sizeof(struct ospf_neighbor));

  n->pool = pool;
  n->ifa = ifa;
  add_tail(&ifa->neigh_list, NODE n);
  n->adj = 0;
  n->csn = 0;
  n->state = NEIGHBOR_DOWN;

  init_lists(n);
  s_init(&(n->dbsi), &(po->lsal));

  n->inactim = tm_new(pool);
  n->inactim->data = n;
  n->inactim->randomize = 0;
  n->inactim->hook = neighbor_timer_hook;
  n->inactim->recurrent = 0;
  DBG("%s: Installing inactivity timer.\n", p->name);

  n->rxmt_timer = tm_new(pool);
  n->rxmt_timer->data = n;
  n->rxmt_timer->randomize = 0;
  n->rxmt_timer->hook = rxmt_timer_hook;
  n->rxmt_timer->recurrent = ifa->rxmtint;
  tm_start(n->rxmt_timer, n->ifa->rxmtint);
  DBG("%s: Installing rxmt timer.\n", p->name);

  n->ackd_timer = tm_new(pool);
  n->ackd_timer->data = n;
  n->ackd_timer->randomize = 0;
  n->ackd_timer->hook = ackd_timer_hook;
  n->ackd_timer->recurrent = ifa->rxmtint / 2;
  init_list(&n->ackl[ACKL_DIRECT]);
  init_list(&n->ackl[ACKL_DELAY]);
  tm_start(n->ackd_timer, n->ifa->rxmtint / 2);
  DBG("%s: Installing ackd timer.\n", p->name);

  return (n);
}
