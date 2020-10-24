static void
if_notify_change(unsigned c, struct iface *i)
{
  struct proto *p;
  struct ifa *a;

  if (i->flags & IF_JUST_CREATED)
    {
      i->flags &= ~IF_JUST_CREATED;
      c |= IF_CHANGE_CREATE | IF_CHANGE_MTU;
    }

  DBG("Interface change notification (%x) for %s\n", c, i->name);
#ifdef LOCAL_DEBUG
  if_dump(i);
#endif

  if (c & IF_CHANGE_UP)
    neigh_if_up(i);

  if (c & IF_CHANGE_DOWN)
    WALK_LIST(a, i->addrs)
      {
	a->flags = (i->flags & ~IA_FLAGS) | (a->flags & IA_FLAGS);
	ifa_notify_change_dep(IF_CHANGE_DOWN, a);
      }

  WALK_LIST(p, active_proto_list)
    if_send_notify(p, c, i);

  if (c & IF_CHANGE_UP)
    WALK_LIST(a, i->addrs)
      {
	a->flags = (i->flags & ~IA_FLAGS) | (a->flags & IA_FLAGS);
	ifa_notify_change_dep(IF_CHANGE_UP, a);
      }

  if ((c & (IF_CHANGE_UP | IF_CHANGE_DOWN | IF_CHANGE_LINK)) == IF_CHANGE_LINK)
    neigh_if_link(i);

  if (c & IF_CHANGE_DOWN)
    neigh_if_down(i);
}
