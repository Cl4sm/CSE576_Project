dev_ifa_notify(struct proto *p, unsigned c, struct ifa *ad)
{
  struct rt_dev_config *P = (void *) p->cf;

  if (!EMPTY_LIST(P->iface_list) &&
      !iface_patt_find(&P->iface_list, ad->iface, ad->iface->addr))
    /* Empty list is automagically treated as "*" */
    return;

  if (ad->flags & IA_SECONDARY)
    return;

  if (ad->scope <= SCOPE_LINK)
    return;

  if (c & IF_CHANGE_DOWN)
    {
      net *n;

      DBG("dev_if_notify: %s:%I going down\n", ad->iface->name, ad->ip);
      n = net_find(p->table, ad->prefix, ad->pxlen);
      if (!n)
	{
	  DBG("dev_if_notify: device shutdown: prefix not found\n");
	  return;
	}
      rte_update(p, n, NULL);
    }
  else if (c & IF_CHANGE_UP)
    {
      rta *a;
      net *n;
      rte *e;

      DBG("dev_if_notify: %s:%I going up\n", ad->iface->name, ad->ip);

      rta a0 = {
	.src = p->main_source,
	.source = RTS_DEVICE,
	.scope = SCOPE_UNIVERSE,
	.cast = RTC_UNICAST,
	.dest = RTD_DEVICE,
	.iface = ad->iface
      };

      a = rta_lookup(&a0);
      n = net_get(p->table, ad->prefix, ad->pxlen);
      e = rte_get_temp(a);
      e->net = n;
      e->pflags = 0;
      rte_update(p, n, e);
    }
}
