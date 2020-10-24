if_update(struct iface *new)
{
  struct iface *i;
  unsigned c;

  WALK_LIST(i, iface_list)
    if (!strcmp(new->name, i->name))
      {
	new->addr = i->addr;
	new->flags = if_recalc_flags(new, new->flags);
	c = if_what_changed(i, new);
	if (c & IF_CHANGE_TOO_MUCH)	/* Changed a lot, convert it to down/up */
	  {
	    DBG("Interface %s changed too much -- forcing down/up transition\n", i->name);
	    if_change_flags(i, i->flags | IF_TMP_DOWN);
	    rem_node(&i->n);
	    new->addr = i->addr;
	    memcpy(&new->addrs, &i->addrs, sizeof(i->addrs));
	    memcpy(i, new, sizeof(*i));
	    i->flags &= ~IF_UP; /* IF_TMP_DOWN will be added later */
	    goto newif;
	  }

	if_copy(i, new);
	if (c)
	  if_notify_change(c, i);

	i->flags |= IF_UPDATED;
	return i;
      }
  i = mb_alloc(if_pool, sizeof(struct iface));
  memcpy(i, new, sizeof(*i));
  init_list(&i->addrs);
newif:
  init_list(&i->neighbors);
  i->flags |= IF_UPDATED | IF_TMP_DOWN;		/* Tmp down as we don't have addresses yet */
  add_tail(&iface_list, &i->n);
  return i;
}
