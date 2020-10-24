static void
rip_tx( sock *s )
{
  struct rip_interface *rif = s->data;
  struct rip_connection *c = rif->busy;
  struct proto *p = c->proto;
  struct rip_packet *packet = (void *) s->tbuf;
  int i, packetlen;
  int maxi, nullupdate = 1;

  DBG( "Sending to %I\n", s->daddr );
  do {

    if (c->done)
      goto done;

    DBG( "Preparing packet to send: " );

    packet->heading.command = RIPCMD_RESPONSE;
#ifndef IPV6
    packet->heading.version = RIP_V2;
#else
    packet->heading.version = RIP_NG;
#endif
    packet->heading.unused  = 0;

    i = !!P_CF->authtype;
#ifndef IPV6
    maxi = ((P_CF->authtype == AT_MD5) ? PACKET_MD5_MAX : PACKET_MAX);
#else
    maxi = 5; /* We need to have at least reserve of one at end of packet */
#endif
    
    FIB_ITERATE_START(&P->rtable, &c->iter, z) {
      struct rip_entry *e = (struct rip_entry *) z;

      if (!rif->triggered || (!(e->updated < now-2))) {		/* FIXME: Should be probably 1 or some different algorithm */
	nullupdate = 0;
	i = rip_tx_prepare( p, packet->block + i, e, rif, i );
	if (i >= maxi) {
	  FIB_ITERATE_PUT(&c->iter, z);
	  goto break_loop;
	}
      }
    } FIB_ITERATE_END(z);
    c->done = 1;

  break_loop:

    packetlen = rip_outgoing_authentication(p, (void *) &packet->block[0], packet, i);

    DBG( ", sending %d blocks, ", i );
    if (nullupdate) {
      DBG( "not sending NULL update\n" );
      c->done = 1;
      goto done;
    }
    if (ipa_nonzero(c->daddr))
      i = sk_send_to( s, packetlen, c->daddr, c->dport );
    else
      i = sk_send( s, packetlen );

    DBG( "it wants more\n" );
  
  } while (i>0);
  
  if (i<0) rip_tx_err( s, i );
  DBG( "blocked\n" );
  return;

done:
  DBG( "Looks like I'm" );
  c->rif->busy = NULL;
  rem_node(NODE c);
  mb_free(c);
  DBG( " done\n" );
  return;
}
