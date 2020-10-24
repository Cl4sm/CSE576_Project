static void 
bfd_session_update_state(struct bfd_session *s, uint state, uint diag)
{
  struct bfd_proto *p = s->ifa->bfd;
  uint old_state = s->loc_state; 
  int notify;

  if (state == old_state)
    return;

  TRACE(D_EVENTS, "Session to %I changed state from %s to %s",
	s->addr, bfd_state_names[old_state], bfd_state_names[state]);

  bfd_lock_sessions(p);
  s->loc_state = state;
  s->loc_diag = diag;

  notify = !NODE_VALID(&s->n);
  if (notify)
    add_tail(&p->notify_list, &s->n);
  bfd_unlock_sessions(p);

  if (state == BFD_STATE_UP)
    bfd_session_set_min_tx(s, s->ifa->cf->min_tx_int);

  if (old_state == BFD_STATE_UP)
    bfd_session_set_min_tx(s, s->ifa->cf->idle_tx_int);

  if (notify)
    bfd_notify_kick(p);
}
