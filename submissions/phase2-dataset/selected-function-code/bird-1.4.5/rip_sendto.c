rip_sendto( struct proto *p, ip_addr daddr, int dport, struct rip_interface *rif )
{
  struct iface *iface = rif->iface;
  struct rip_connection *c;
  static int num = 0;

  if (rif->busy) {
    log (L_WARN "%s: Interface %s is much too slow, dropping request", p->name, iface->name);
    return;
  }
  c = mb_alloc( p->pool, sizeof( struct rip_connection ));
  rif->busy = c;
  
  c->addr = daddr;
  c->proto = p;
  c->num = num++;
  c->rif = rif;

  c->dport = dport;
  c->daddr = daddr;
  if (c->rif->sock->data != rif)
    bug("not enough send magic");

  c->done = 0;
  FIB_ITERATE_INIT( &c->iter, &P->rtable );
  add_head( &P->connections, NODE c );
  if (ipa_nonzero(daddr))
    TRACE(D_PACKETS, "Sending my routing table to %I:%d on %s", daddr, dport, rif->iface->name );
  else
    TRACE(D_PACKETS, "Broadcasting routing table to %s", rif->iface->name );

  rip_tx(c->rif->sock);
}
