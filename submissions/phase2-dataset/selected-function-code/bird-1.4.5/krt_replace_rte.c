void
krt_replace_rte(struct krt_proto *p, net *n, rte *new, rte *old, struct ea_list *eattrs)
{
  int err = 0;

  /*
   * NULL for eattr of the old route is a little hack, but we don't
   * get proper eattrs for old in rt_notify() anyway. NULL means no
   * extended route attributes and therefore matches if the kernel
   * route has any of them.
   */

  if (old)
    nl_send_route(p, old, NULL, 0);

  if (new)
    err = nl_send_route(p, new, eattrs, 1);

  if (err < 0)
    n->n.flags |= KRF_SYNC_ERROR;
  else
    n->n.flags &= ~KRF_SYNC_ERROR;
}
