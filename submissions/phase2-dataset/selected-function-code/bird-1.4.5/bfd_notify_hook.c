static int
bfd_notify_hook(sock *sk, int len)
{
  struct bfd_proto *p = sk->data;
  struct bfd_session *s;
  list tmp_list;
  u8 state, diag;
  node *n, *nn;

  pipe_drain(sk->fd);

  bfd_lock_sessions(p);
  init_list(&tmp_list);
  add_tail_list(&tmp_list, &p->notify_list);
  init_list(&p->notify_list);
  bfd_unlock_sessions(p);

  WALK_LIST_FIRST(s, tmp_list)
  {
    bfd_lock_sessions(p);
    rem2_node(&s->n);
    state = s->loc_state;
    diag = s->loc_diag;
    bfd_unlock_sessions(p);

    /* FIXME: convert to btime and move to bfd_session_update_state() */ 
    s->last_state_change = now;

    s->notify_running = 1;
    WALK_LIST_DELSAFE(n, nn, s->request_list)
      bfd_request_notify(SKIP_BACK(struct bfd_request, n, n), state, diag);
    s->notify_running = 0;

    /* Remove the session if all requests were removed in notify hooks */
    if (EMPTY_LIST(s->request_list))
      bfd_remove_session(p, s);
  }

  return 0;
}
