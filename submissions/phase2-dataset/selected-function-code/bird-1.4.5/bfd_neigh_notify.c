bfd_neigh_notify(struct neighbor *nb)
{
  struct bfd_proto *p = (struct bfd_proto *) nb->proto;
  struct bfd_neighbor *n = nb->data;

  if (!n)
    return;

  if ((nb->scope > 0) && !n->req)
  {
    ip_addr local = ipa_nonzero(n->local) ? n->local : nb->ifa->ip;
    n->req = bfd_request_session(p->p.pool, n->addr, local, nb->iface, NULL, NULL);
  }

  if ((nb->scope <= 0) && n->req)
  {
    rfree(n->req);
    n->req = NULL;
  }
}
