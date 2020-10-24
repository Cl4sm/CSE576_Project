static void
radv_iface_new(struct proto_radv *ra, struct iface *iface, struct radv_iface_config *cf)
{
  pool *pool = ra->p.pool;
  struct radv_iface *ifa;

  RADV_TRACE(D_EVENTS, "Adding interface %s", iface->name);

  ifa = mb_allocz(pool, sizeof(struct radv_iface));
  ifa->ra = ra;
  ifa->cf = cf;
  ifa->iface = iface;

  add_tail(&ra->iface_list, NODE ifa);

  ifa->addr = find_lladdr(iface);
  if (!ifa->addr)
  {
    log(L_ERR "%s: Cannot find link-locad addr on interface %s", ra->p.name, iface->name);
    return;
  }

  timer *tm = tm_new(pool);
  tm->hook = radv_timer;
  tm->data = ifa;
  tm->randomize = 0;
  tm->recurrent = 0;
  ifa->timer = tm;

  struct object_lock *lock = olock_new(pool);
  lock->addr = IPA_NONE;
  lock->type = OBJLOCK_IP;
  lock->port = ICMPV6_PROTO;
  lock->iface = iface;
  lock->data = ifa;
  lock->hook = radv_iface_add;
  ifa->lock = lock;

  olock_acquire(lock);
}
