struct ifa *
ifa_update(struct ifa *a)
{
  struct iface *i = a->iface;
  struct ifa *b;

  WALK_LIST(b, i->addrs)
    if (ifa_same(b, a))
      {
	if (ipa_equal(b->brd, a->brd) &&
	    ipa_equal(b->opposite, a->opposite) &&
	    b->scope == a->scope &&
	    !((b->flags ^ a->flags) & IA_PEER))
	  {
	    b->flags |= IF_UPDATED;
	    return b;
	  }
	ifa_delete(b);
	break;
      }

#ifndef IPV6
  if ((i->flags & IF_BROADCAST) && !ipa_nonzero(a->brd))
    log(L_ERR "Missing broadcast address for interface %s", i->name);
#endif

  b = mb_alloc(if_pool, sizeof(struct ifa));
  memcpy(b, a, sizeof(struct ifa));
  add_tail(&i->addrs, &b->n);
  b->flags = (i->flags & ~IA_FLAGS) | (a->flags & IA_FLAGS);
  if (ifa_recalc_primary(i))
    if_change_flags(i, i->flags | IF_TMP_DOWN);
  if (b->flags & IF_UP)
    ifa_notify_change(IF_CHANGE_CREATE | IF_CHANGE_UP, b);
  return b;
}
