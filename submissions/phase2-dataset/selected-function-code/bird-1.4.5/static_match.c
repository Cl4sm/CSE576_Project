static void
static_match(struct proto *p, struct static_route *r, struct static_config *n)
{
  struct static_route *t;

  /*
   * For given old route *r we find whether a route to the same
   * network is also in the new route list. In that case, we keep the
   * route and possibly update the route later if destination changed.
   * Otherwise, we remove the route.
   */

  if (r->neigh)
    r->neigh->data = NULL;

  WALK_LIST(t, n->iface_routes)
    if (static_same_net(r, t))
      goto found;

  WALK_LIST(t, n->other_routes)
    if (static_same_net(r, t))
      goto found;

  static_remove(p, r);
  return;

 found:
  /* If destination is different, force reinstall */
  if ((r->installed > 0) && !static_same_dest(r, t))
    t->installed = -1;
  else
    t->installed = r->installed;
}
