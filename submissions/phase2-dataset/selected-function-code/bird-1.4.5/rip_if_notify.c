rip_if_notify(struct proto *p, unsigned c, struct iface *iface)
{
  DBG( "RIP: if notify\n" );
  if (iface->flags & IF_IGNORE)
    return;
  if (c & IF_CHANGE_DOWN) {
    struct rip_interface *i;
    i = find_interface(p, iface);
    if (i) {
      rem_node(NODE i);
      rfree(i->lock);
      kill_iface(i);
    }
  }
  if (c & IF_CHANGE_UP) {
    struct iface_patt *k = iface_patt_find(&P_CF->iface_list, iface, iface->addr);
    struct object_lock *lock;
    struct rip_patt *PATT = (struct rip_patt *) k;

    if (!k) return; /* We are not interested in this interface */

    lock = olock_new( p->pool );
    if (!(PATT->mode & IM_BROADCAST) && (iface->flags & IF_MULTICAST))
#ifndef IPV6
      lock->addr = ipa_from_u32(0xe0000009);
#else
      ip_pton("FF02::9", &lock->addr);
#endif
    else
      lock->addr = iface->addr->brd;
    lock->port = P_CF->port;
    lock->iface = iface;
    lock->hook = rip_real_if_add;
    lock->data = p;
    lock->type = OBJLOCK_UDP;
    olock_acquire(lock);
  }
}
