rip_process_packet( struct proto *p, struct rip_packet *packet, int num, ip_addr whotoldme, int port, struct iface *iface )
{
  int i;
  int authenticated = 0;
  neighbor *neighbor;

  switch( packet->heading.version ) {
  case RIP_V1: DBG( "Rip1: " ); break;
  case RIP_V2: DBG( "Rip2: " ); break;
  default: BAD( "Unknown version" );
  }

  switch( packet->heading.command ) {
  case RIPCMD_REQUEST: DBG( "Asked to send my routing table\n" ); 
	  if (P_CF->honor == HO_NEVER)
	    BAD( "They asked me to send routing table, but I was told not to do it" );

	  if ((P_CF->honor == HO_NEIGHBOR) && (!neigh_find2( p, &whotoldme, iface, 0 )))
	    BAD( "They asked me to send routing table, but he is not my neighbor" );
    	  rip_sendto( p, whotoldme, port, HEAD(P->interfaces) ); /* no broadcast */
          break;
  case RIPCMD_RESPONSE: DBG( "*** Rtable from %I\n", whotoldme ); 
          if (port != P_CF->port) {
	    log( L_REMOTE "%s: %I send me routing info from port %d", p->name, whotoldme, port );
	    return 1;
	  }

	  if (!(neighbor = neigh_find2( p, &whotoldme, iface, 0 )) || neighbor->scope == SCOPE_HOST) {
	    log( L_REMOTE "%s: %I send me routing info but he is not my neighbor", p->name, whotoldme );
	    return 0;
	  }

          for (i=0; i<num; i++) {
	    struct rip_block *block = &packet->block[i];
#ifndef IPV6
	    /* Authentication is not defined for v6 */
	    if (block->family == 0xffff) {
	      if (i)
		continue;	/* md5 tail has this family */
	      if (rip_incoming_authentication(p, (void *) block, packet, num, whotoldme))
		BAD( "Authentication failed" );
	      authenticated = 1;
	      continue;
	    }
#endif
	    if ((!authenticated) && (P_CF->authtype != AT_NONE))
	      BAD( "Packet is not authenticated and it should be" );
	    ipa_ntoh( block->network );
#ifndef IPV6
	    ipa_ntoh( block->netmask );
	    ipa_ntoh( block->nexthop );
	    if (packet->heading.version == RIP_V1)	/* FIXME (nonurgent): switch to disable this? */
	      block->netmask = ipa_class_mask(block->network);
#endif
	    process_block( p, block, whotoldme, iface );
	  }
          break;
  case RIPCMD_TRACEON:
  case RIPCMD_TRACEOFF: BAD( "I was asked for traceon/traceoff" );
  case 5: BAD( "Some Sun extension around here" );
  default: BAD( "Unknown command" );
  }

  return 0;
}
