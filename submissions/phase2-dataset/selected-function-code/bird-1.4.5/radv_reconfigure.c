radv_reconfigure(struct proto *p, struct proto_config *c)
{
  struct proto_radv *ra = (struct proto_radv *) p;
  // struct radv_config *old = (struct radv_config *) (p->cf);
  struct radv_config *new = (struct radv_config *) c;

  /* 
   * The question is why there is a reconfigure function for RAdv if
   * it has almost none internal state so restarting the protocol
   * would probably suffice. One small reason is that restarting the
   * protocol would lead to sending a RA with Router Lifetime 0
   * causing nodes to temporary remove their default routes.
   */

  p->cf = c; /* radv_check_active() requires proper p->cf */
  ra->active = radv_check_active(ra);

  struct iface *iface;
  WALK_LIST(iface, iface_list)
  {
    struct radv_iface *ifa = radv_iface_find(ra, iface);
    struct radv_iface_config *ic = (struct radv_iface_config *)
      iface_patt_find(&new->patt_list, iface, NULL);

    if (ifa && ic)
    {
      ifa->cf = ic;

      /* We cheat here - always notify the change even if there isn't
	 any. That would leads just to a few unnecessary RAs. */
      radv_iface_notify(ifa, RA_EV_CHANGE);
    }

    if (ifa && !ic)
    {
      radv_iface_shutdown(ifa);
      radv_iface_remove(ifa);
    }

    if (!ifa && ic)
      radv_iface_new(ra, iface, ic);
  }

  return 1;
}
