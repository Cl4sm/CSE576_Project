if_dump(struct iface *i)
{
  struct ifa *a;

  debug("IF%d: %s", i->index, i->name);
  if (i->flags & IF_SHUTDOWN)
    debug(" SHUTDOWN");
  if (i->flags & IF_UP)
    debug(" UP");
  else
    debug(" DOWN");
  if (i->flags & IF_ADMIN_UP)
    debug(" LINK-UP");
  if (i->flags & IF_MULTIACCESS)
    debug(" MA");
  if (i->flags & IF_BROADCAST)
    debug(" BC");
  if (i->flags & IF_MULTICAST)
    debug(" MC");
  if (i->flags & IF_LOOPBACK)
    debug(" LOOP");
  if (i->flags & IF_IGNORE)
    debug(" IGN");
  if (i->flags & IF_TMP_DOWN)
    debug(" TDOWN");
  debug(" MTU=%d\n", i->mtu);
  WALK_LIST(a, i->addrs)
    {
      ifa_dump(a);
      ASSERT((a != i->addr) == !(a->flags & IA_PRIMARY));
    }
}
