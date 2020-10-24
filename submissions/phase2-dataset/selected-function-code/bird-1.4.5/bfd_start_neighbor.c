bfd_start_neighbor(struct bfd_proto *p, struct bfd_neighbor *n)
{
  n->active = 1;

  if (n->multihop)
  {
    n->req = bfd_request_session(p->p.pool, n->addr, n->local, NULL, NULL, NULL);
    return;
  }

  struct neighbor *nb = neigh_find2(&p->p, &n->addr, n->iface, NEF_STICKY);
  if (!nb)
  {
    log(L_ERR "%s: Invalid remote address %I%J", p->p.name, n->addr, n->iface);
    return;
  }

  if (nb->data)
  {
    log(L_ERR "%s: Duplicate neighbor %I", p->p.name, n->addr);
    return;
  }

  n->neigh = nb;
  nb->data = n;

  if (nb->scope > 0)
    bfd_neigh_notify(nb);
  else
    TRACE(D_EVENTS, "Waiting for %I%J to become my neighbor", n->addr, n->iface);
}
