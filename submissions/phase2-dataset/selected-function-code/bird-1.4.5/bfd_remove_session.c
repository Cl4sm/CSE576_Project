bfd_remove_session(struct bfd_proto *p, struct bfd_session *s)
{
  ip_addr ip = s->addr;

  /* Caller should ensure that request list is empty */

  birdloop_enter(p->loop);

  /* Remove session from notify list if scheduled for notification */
  /* No need for bfd_lock_sessions(), we are already protected by birdloop_enter() */
  if (NODE_VALID(&s->n))
    rem_node(&s->n);

  bfd_free_iface(s->ifa);

  rfree(s->tx_timer);
  rfree(s->hold_timer);

  HASH_REMOVE(p->session_hash_id, HASH_ID, s);
  HASH_REMOVE(p->session_hash_ip, HASH_IP, s);

  sl_free(p->session_slab, s);

  TRACE(D_EVENTS, "Session to %I removed", ip);

  birdloop_leave(p->loop);
}
