bfd_request_session(pool *p, ip_addr addr, ip_addr local, struct iface *iface,
		    void (*hook)(struct bfd_request *), void *data)
{
  struct bfd_request *req = ralloc(p, &bfd_request_class);

  /* Hack: self-link req->n, we will call rem_node() on it */
  req->n.prev = req->n.next = &req->n;

  req->addr = addr;
  req->local = local;
  req->iface = iface;

  bfd_submit_request(req);

  req->hook = hook;
  req->data = data;

  return req;
}
