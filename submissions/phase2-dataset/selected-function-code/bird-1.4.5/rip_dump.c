rip_dump(struct proto *p)
{
  int i;
  node *w;
  struct rip_interface *rif;

  CHK_MAGIC;
  WALK_LIST( w, P->connections ) {
    struct rip_connection *n = (void *) w;
    debug( "RIP: connection #%d: %I\n", n->num, n->addr );
  }
  i = 0;
  FIB_WALK( &P->rtable, e ) {
    debug( "RIP: entry #%d: ", i++ );
    rip_dump_entry( (struct rip_entry *)e );
  } FIB_WALK_END;
  i = 0;
  WALK_LIST( rif, P->interfaces ) {
    debug( "RIP: interface #%d: %s, %I, busy = %x\n", i++, rif->iface?rif->iface->name:"(dummy)", rif->sock->daddr, rif->busy );
  }
}
