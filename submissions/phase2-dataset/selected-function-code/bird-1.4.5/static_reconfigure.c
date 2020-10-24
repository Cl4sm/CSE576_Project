static int
static_reconfigure(struct proto *p, struct proto_config *new)
{
  struct static_config *o = (void *) p->cf;
  struct static_config *n = (void *) new;
  struct static_route *r;

  if (cf_igp_table(o) != cf_igp_table(n))
    return 0;

  /* Delete all obsolete routes and reset neighbor entries */
  WALK_LIST(r, o->iface_routes)
    static_match(p, r, n);
  WALK_LIST(r, o->other_routes)
    static_match(p, r, n);

  /* Now add all new routes, those not changed will be ignored by static_install() */
  WALK_LIST(r, n->iface_routes)
    {
      struct iface *ifa;
      if ((ifa = if_find_by_name(r->if_name)) && (ifa->flags & IF_UP))
	static_install(p, r, ifa);
    }
  WALK_LIST(r, n->other_routes)
    static_add(p, n, r);

  return 1;
}
