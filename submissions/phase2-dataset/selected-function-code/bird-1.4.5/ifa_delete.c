void
ifa_delete(struct ifa *a)
{
  struct iface *i = a->iface;
  struct ifa *b;

  WALK_LIST(b, i->addrs)
    if (ifa_same(b, a))
      {
	rem_node(&b->n);
	if (b->flags & IF_UP)
	  {
	    b->flags &= ~IF_UP;
	    ifa_notify_change(IF_CHANGE_DOWN, b);
	  }
	if (b->flags & IA_PRIMARY)
	  {
	    if_change_flags(i, i->flags | IF_TMP_DOWN);
	    ifa_recalc_primary(i);
	  }
	mb_free(b);
	return;
      }
}
