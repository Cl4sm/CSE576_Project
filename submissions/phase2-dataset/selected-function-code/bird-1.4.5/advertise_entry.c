advertise_entry( struct proto *p, struct rip_block *b, ip_addr whotoldme, struct iface *iface )
{
  rta *a, A;
  rte *r;
  net *n;
  neighbor *neighbor;
  struct rip_interface *rif;
  int pxlen;

  bzero(&A, sizeof(A));
  A.src= p->main_source;
  A.source = RTS_RIP;
  A.scope = SCOPE_UNIVERSE;
  A.cast = RTC_UNICAST;
  A.dest = RTD_ROUTER;
  A.flags = 0;
#ifndef IPV6
  A.gw = ipa_nonzero(b->nexthop) ? b->nexthop : whotoldme;
  pxlen = ipa_mklen(b->netmask);
#else
  /* FIXME: next hop is in other packet for v6 */
  A.gw = whotoldme; 
  pxlen = b->pxlen;
#endif
  A.from = whotoldme;

  /* No need to look if destination looks valid - ie not net 0 or 127 -- core will do for us. */

  neighbor = neigh_find2( p, &A.gw, iface, 0 );
  if (!neighbor) {
    log( L_REMOTE "%s: %I asked me to route %I/%d using not-neighbor %I.", p->name, A.from, b->network, pxlen, A.gw );
    return;
  }
  if (neighbor->scope == SCOPE_HOST) {
    DBG("Self-destined route, ignoring.\n");
    return;
  }

  A.iface = neighbor->iface;
  if (!(rif = neighbor->data)) {
    rif = neighbor->data = find_interface(p, A.iface);
  }
  if (!rif)
    bug("Route packet using unknown interface? No.");
    
  /* set to: interface of nexthop */
  a = rta_lookup(&A);
  if (pxlen==-1)  {
    log( L_REMOTE "%s: %I gave me invalid pxlen/netmask for %I.", p->name, A.from, b->network );
    return;
  }
  n = net_get( p->table, b->network, pxlen );
  r = rte_get_temp(a);
#ifndef IPV6
  r->u.rip.metric = ntohl(b->metric) + rif->metric;
#else  
  r->u.rip.metric = b->metric + rif->metric;
#endif

  r->u.rip.entry = NULL;
  if (r->u.rip.metric > P_CF->infinity) r->u.rip.metric = P_CF->infinity;
  r->u.rip.tag = ntohl(b->tag);
  r->net = n;
  r->pflags = 0; /* Here go my flags */
  rip_rte_update_if_better( p->table, n, p, r );
  DBG( "done\n" );
}
