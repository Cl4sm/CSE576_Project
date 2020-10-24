radv_rt_notify(struct proto *p, rtable *tbl UNUSED, net *n, rte *new, rte *old UNUSED, ea_list *attrs UNUSED)
{
  struct proto_radv *ra = (struct proto_radv *) p;
  struct radv_config *cf = (struct radv_config *) (p->cf);

  if (radv_net_match_trigger(cf, n))
  {
    u8 old_active = ra->active;
    ra->active = !!new;

    if (ra->active == old_active)
      return;

    if (ra->active)
      RADV_TRACE(D_EVENTS, "Triggered");
    else
      RADV_TRACE(D_EVENTS, "Suppressed");

    radv_iface_notify_all(ra, RA_EV_CHANGE);
  }
}
