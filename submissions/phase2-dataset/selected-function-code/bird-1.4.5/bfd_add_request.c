static int
bfd_add_request(struct bfd_proto *p, struct bfd_request *req)
{
  struct bfd_session *s = bfd_find_session_by_addr(p, req->addr);
  u8 state, diag;

  if (!s)
    s = bfd_add_session(p, req->addr, req->local, req->iface);

  rem_node(&req->n);
  add_tail(&s->request_list, &req->n);
  req->session = s;

  bfd_lock_sessions(p);
  state = s->loc_state;
  diag = s->loc_diag;
  bfd_unlock_sessions(p);

  bfd_request_notify(req, state, diag);

  return 1;
}
