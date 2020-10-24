static void
radv_if_notify(struct proto *p, unsigned flags, struct iface *iface)
{ 
  struct proto_radv *ra = (struct proto_radv *) p;
  struct radv_config *cf = (struct radv_config *) (p->cf);

  if (iface->flags & IF_IGNORE)
    return;

  if (flags & IF_CHANGE_UP)
  {
    struct radv_iface_config *ic = (struct radv_iface_config *)
      iface_patt_find(&cf->patt_list, iface, NULL);

    if (ic)
      radv_iface_new(ra, iface, ic);

    return;
  }

  struct radv_iface *ifa = radv_iface_find(ra, iface);
  if (!ifa)
    return;

  if (flags & IF_CHANGE_DOWN)
  {
    radv_iface_remove(ifa);
    return;
  }

  if ((flags & IF_CHANGE_LINK) && (iface->flags & IF_LINK_UP))
    radv_iface_notify(ifa, RA_EV_INIT);
}
