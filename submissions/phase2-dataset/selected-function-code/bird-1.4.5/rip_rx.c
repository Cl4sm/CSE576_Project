static int
rip_rx(sock *s, int size)
{
  struct rip_interface *i = s->data;
  struct proto *p = i->proto;
  struct iface *iface = NULL;
  int num;

  /* In non-listening mode, just ignore packet */
  if (i->mode & IM_NOLISTEN)
    return 1;

#ifdef IPV6
  if (! i->iface || s->lifindex != i->iface->index)
    return 1;

  iface = i->iface;
#endif

  if (i->check_ttl && (s->rcv_ttl < 255))
  {
    log( L_REMOTE "%s: Discarding packet with TTL %d (< 255) from %I on %s",
	 p->name, s->rcv_ttl, s->faddr, i->iface->name);
    return 1;
  }


  CHK_MAGIC;
  DBG( "RIP: message came: %d bytes from %I via %s\n", size, s->faddr, i->iface ? i->iface->name : "(dummy)" );
  size -= sizeof( struct rip_packet_heading );
  if (size < 0) BAD( "Too small packet" );
  if (size % sizeof( struct rip_block )) BAD( "Odd sized packet" );
  num = size / sizeof( struct rip_block );
  if (num>PACKET_MAX) BAD( "Too many blocks" );

  if (ipa_equal(i->iface->addr->ip, s->faddr)) {
    DBG("My own packet\n");
    return 1;
  }

  rip_process_packet( p, (struct rip_packet *) s->rbuf, num, s->faddr, s->fport, iface );
  return 1;
}
