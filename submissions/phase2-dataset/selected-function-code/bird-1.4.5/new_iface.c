new_iface(struct proto *p, struct iface *new, unsigned long flags, struct iface_patt *patt )
{
  struct rip_interface *rif;
  struct rip_patt *PATT = (struct rip_patt *) patt;

  rif = mb_allocz(p->pool, sizeof( struct rip_interface ));
  rif->iface = new;
  rif->proto = p;
  rif->busy = NULL;
  if (PATT) {
    rif->mode = PATT->mode;
    rif->metric = PATT->metric;
    rif->multicast = (!(PATT->mode & IM_BROADCAST)) && (flags & IF_MULTICAST);
    rif->check_ttl = (PATT->ttl_security == 1);
  }
  /* lookup multicasts over unnumbered links - no: rip is not defined over unnumbered links */

  if (rif->multicast)
    DBG( "Doing multicasts!\n" );

  rif->sock = sk_new( p->pool );
  rif->sock->type = SK_UDP;
  rif->sock->sport = P_CF->port;
  rif->sock->rx_hook = rip_rx;
  rif->sock->data = rif;
  rif->sock->rbsize = 10240;
  rif->sock->iface = new;		/* Automagically works for dummy interface */
  rif->sock->tbuf = mb_alloc( p->pool, sizeof( struct rip_packet ));
  rif->sock->tx_hook = rip_tx;
  rif->sock->err_hook = rip_tx_err;
  rif->sock->daddr = IPA_NONE;
  rif->sock->dport = P_CF->port;
  if (new)
    {
      rif->sock->tos = PATT->tx_tos;
      rif->sock->priority = PATT->tx_priority;
      rif->sock->ttl = PATT->ttl_security ? 255 : 1;
      rif->sock->flags = SKF_LADDR_RX | (rif->check_ttl ? SKF_TTL_RX : 0);
    }

  if (new) {
    if (new->addr->flags & IA_PEER)
      log( L_WARN "%s: rip is not defined over unnumbered links", p->name );
    if (rif->multicast) {
#ifndef IPV6
      rif->sock->daddr = ipa_from_u32(0xe0000009);
#else
      rif->sock->daddr = ipa_build(0xff020000, 0, 0, 9);
#endif
    } else {
      rif->sock->daddr = new->addr->brd;
    }
  }

  if (!ipa_nonzero(rif->sock->daddr)) {
    if (rif->iface)
      log( L_WARN "%s: interface %s is too strange for me", p->name, rif->iface->name );
  } else {

    if (sk_open(rif->sock) < 0)
      goto err;

    if (rif->multicast)
      {
	if (sk_setup_multicast(rif->sock) < 0)
	  goto err;
	if (sk_join_group(rif->sock, rif->sock->daddr) < 0)
	  goto err;
      }
    else
      {
	if (sk_setup_broadcast(rif->sock) < 0)
	  goto err;
      }
  }

  TRACE(D_EVENTS, "Listening on %s, port %d, mode %s (%I)", rif->iface ? rif->iface->name : "(dummy)", P_CF->port, rif->multicast ? "multicast" : "broadcast", rif->sock->daddr );
  
  return rif;

 err:
  sk_log_error(rif->sock, p->name);
  log(L_ERR "%s: Cannot open socket for %s", p->name, rif->iface ? rif->iface->name : "(dummy)" );
  if (rif->iface) {
    rfree(rif->sock);
    mb_free(rif);
    return NULL;
  }
  /* On dummy, we just return non-working socket, so that user gets error every time anyone requests table */
  return rif;
}
