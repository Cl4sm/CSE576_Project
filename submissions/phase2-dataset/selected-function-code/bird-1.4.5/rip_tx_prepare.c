rip_tx_prepare(struct proto *p, struct rip_block *b, struct rip_entry *e, struct rip_interface *rif, int pos )
{
  int metric;
  DBG( "." );
  b->tag     = htons( e->tag );
  b->network = e->n.prefix;
  metric = e->metric;
  if (neigh_connected_to(p, &e->whotoldme, rif->iface)) {
    DBG( "(split horizon)" );
    metric = P_CF->infinity;
  }
#ifndef IPV6
  b->family  = htons( 2 ); /* AF_INET */
  b->netmask = ipa_mkmask( e->n.pxlen );
  ipa_hton( b->netmask );

  if (neigh_connected_to(p, &e->nexthop, rif->iface))
    b->nexthop = e->nexthop;
  else
    b->nexthop = IPA_NONE;
  ipa_hton( b->nexthop );  
  b->metric  = htonl( metric );
#else
  b->pxlen = e->n.pxlen;
  b->metric  = metric; /* it is u8 */
#endif

  ipa_hton( b->network );

  return pos+1;
}
