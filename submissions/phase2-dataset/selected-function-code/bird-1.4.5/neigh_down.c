neigh_down(neighbor *n)
{
  DBG("Flushing neighbor %I on %s\n", n->addr, n->iface->name);
  rem_node(&n->if_n);
  if (! (n->flags & NEF_BIND))
    n->iface = NULL;
  n->ifa = NULL;
  n->scope = -1;
  if (n->proto->neigh_notify && n->proto->core_state != FS_FLUSHING)
    n->proto->neigh_notify(n);
  rem_node(&n->n);
  if (n->flags & NEF_STICKY)
    {
      add_tail(&sticky_neigh_list, &n->n);

      /* Respawn neighbor if there is another matching prefix */
      struct iface *i;
      struct ifa *a;
      int scope;

      if (!n->iface)
	WALK_LIST(i, iface_list)
	  if ((scope = if_connected(&n->addr, i, &a)) >= 0)
	    {
	      neigh_up(n, i, scope, a);
	      return;
	    }
    }
  else
    sl_free(neigh_slab, n);
}
