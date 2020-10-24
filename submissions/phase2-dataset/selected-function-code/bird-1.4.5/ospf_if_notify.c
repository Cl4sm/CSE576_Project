ospf_if_notify(struct proto *p, unsigned flags, struct iface *iface)
{
  struct proto_ospf *po = (struct proto_ospf *) p;

  /*
  if (iface->flags & IF_IGNORE)
    return;
  */

  /* Going up means that there are no such ifaces yet */
  if (flags & IF_CHANGE_UP)
    return;

  struct ospf_iface *ifa, *ifx;
  WALK_LIST_DELSAFE(ifa, ifx, po->iface_list)
    if (ifa->iface == iface)
      ospf_iface_notify(po, flags, ifa);

  /* We use here that even shutting down iface also shuts down
     the vlinks, but vlinks are not freed and stays in the
     iface_list even when down */
}
