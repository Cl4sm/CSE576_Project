static int
bgp_start(struct proto *P)
{
  struct bgp_proto *p = (struct bgp_proto *) P;
  struct object_lock *lock;

  DBG("BGP: Startup.\n");
  p->start_state = BSS_PREPARE;
  p->outgoing_conn.state = BS_IDLE;
  p->incoming_conn.state = BS_IDLE;
  p->neigh = NULL;
  p->bfd_req = NULL;
  p->gr_ready = 0;
  p->gr_active = 0;

  rt_lock_table(p->igp_table);

  p->event = ev_new(p->p.pool);
  p->event->hook = bgp_decision;
  p->event->data = p;

  p->startup_timer = tm_new(p->p.pool);
  p->startup_timer->hook = bgp_startup_timeout;
  p->startup_timer->data = p;

  p->gr_timer = tm_new(p->p.pool);
  p->gr_timer->hook = bgp_graceful_restart_timeout;
  p->gr_timer->data = p;

  p->local_id = proto_get_router_id(P->cf);
  if (p->rr_client)
    p->rr_cluster_id = p->cf->rr_cluster_id ? p->cf->rr_cluster_id : p->local_id;

  p->remote_id = 0;
  p->source_addr = p->cf->source_addr;

  if (p->p.gr_recovery && p->cf->gr_mode)
    proto_graceful_restart_lock(P);

  /*
   *  Before attempting to create the connection, we need to lock the
   *  port, so that are sure we're the only instance attempting to talk
   *  with that neighbor.
   */

  lock = p->lock = olock_new(P->pool);
  lock->addr = p->cf->remote_ip;
  lock->port = p->cf->remote_port;
  lock->iface = p->cf->iface;
  lock->type = OBJLOCK_TCP;
  lock->hook = bgp_start_locked;
  lock->data = p;
  olock_acquire(lock);

  return PS_START;
}
