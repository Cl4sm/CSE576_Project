ifa_recalc_primary(struct iface *i)
{
  struct ifa *a = kif_choose_primary(i);

  if (a == i->addr)
    return 0;

  if (i->addr)
    i->addr->flags &= ~IA_PRIMARY;

  if (a)
    {
      a->flags |= IA_PRIMARY;
      rem_node(&a->n);
      add_head(&i->addrs, &a->n);
    }

  i->addr = a;
  return 1;
}
