static void
process_block( struct proto *p, struct rip_block *block, ip_addr whotoldme, struct iface *iface )
{
  int metric, pxlen;

#ifndef IPV6
  metric = ntohl( block->metric );
  pxlen = ipa_mklen(block->netmask);
#else
  metric = block->metric;
  pxlen = block->pxlen;
#endif
  ip_addr network = block->network;

  CHK_MAGIC;

  TRACE(D_ROUTES, "block: %I tells me: %I/%d available, metric %d... ",
      whotoldme, network, pxlen, metric );

  if ((!metric) || (metric > P_CF->infinity)) {
#ifdef IPV6 /* Someone is sending us nexthop and we are ignoring it */
    if (metric == 0xff)
      { DBG( "IPv6 nexthop ignored" ); return; }
#endif
    log( L_WARN "%s: Got metric %d from %I", p->name, metric, whotoldme );
    return;
  }

  advertise_entry( p, block, whotoldme, iface );
}
