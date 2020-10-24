static int
rip_start(struct proto *p)
{
  struct rip_interface *rif;
  DBG( "RIP: starting instance...\n" );

  ASSERT(sizeof(struct rip_packet_heading) == 4);
  ASSERT(sizeof(struct rip_block) == 20);
  ASSERT(sizeof(struct rip_block_auth) == 20);

#ifdef LOCAL_DEBUG
  P->magic = RIP_MAGIC;
#endif
  fib_init( &P->rtable, p->pool, sizeof( struct rip_entry ), 0, NULL );
  init_list( &P->connections );
  init_list( &P->garbage );
  init_list( &P->interfaces );
  P->timer = tm_new( p->pool );
  P->timer->data = p;
  P->timer->recurrent = 1;
  P->timer->hook = rip_timer;
  tm_start( P->timer, 2 );
  rif = new_iface(p, NULL, 0, NULL);	/* Initialize dummy interface */
  add_head( &P->interfaces, NODE rif );
  CHK_MAGIC;

  DBG( "RIP: ...done\n");
  return PS_UP;
}
