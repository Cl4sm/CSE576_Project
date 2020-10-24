bgp_init(struct proto_config *C)
{
  struct proto *P = proto_new(C, sizeof(struct bgp_proto));
  struct bgp_config *c = (struct bgp_config *) C;
  struct bgp_proto *p = (struct bgp_proto *) P;

  P->accept_ra_types = c->secondary ? RA_ACCEPTED : RA_OPTIMAL;
  P->rt_notify = bgp_rt_notify;
  P->import_control = bgp_import_control;
  P->neigh_notify = bgp_neigh_notify;
  P->reload_routes = bgp_reload_routes;
  P->feed_done = bgp_feed_done;
  P->rte_better = bgp_rte_better;
  P->rte_recalculate = c->deterministic_med ? bgp_rte_recalculate : NULL;

  p->cf = c;
  p->local_as = c->local_as;
  p->remote_as = c->remote_as;
  p->is_internal = (c->local_as == c->remote_as);
  p->rs_client = c->rs_client;
  p->rr_client = c->rr_client;
  p->igp_table = get_igp_table(c);

  return P;
}
