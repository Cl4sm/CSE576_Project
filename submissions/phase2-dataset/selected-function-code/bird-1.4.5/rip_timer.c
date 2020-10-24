rip_timer(timer *t)
{
  struct proto *p = t->data;
  struct fib_node *e, *et;

  CHK_MAGIC;
  DBG( "RIP: tick tock\n" );
  
  WALK_LIST_DELSAFE( e, et, P->garbage ) {
    rte *rte;
    rte = SKIP_BACK( struct rte, u.rip.garbage, e );

    CHK_MAGIC;

    DBG( "Garbage: (%p)", rte ); rte_dump( rte );

    if (now - rte->lastmod > P_CF->timeout_time) {
      TRACE(D_EVENTS, "entry is too old: %I", rte->net->n.prefix );
      if (rte->u.rip.entry) {
	rte->u.rip.entry->metric = P_CF->infinity;
	rte->u.rip.metric = P_CF->infinity;
      }
    }

    if (now - rte->lastmod > P_CF->garbage_time) {
      TRACE(D_EVENTS, "entry is much too old: %I", rte->net->n.prefix );
      rte_discard(p->table, rte);
    }
  }

  DBG( "RIP: Broadcasting routing tables\n" );
  {
    struct rip_interface *rif;

    if ( P_CF->period > 2 ) {		/* Bring some randomness into sending times */
      if (! (P->tx_count % P_CF->period)) P->rnd_count = random_u32() % 2;
    } else P->rnd_count = P->tx_count % P_CF->period;

    WALK_LIST( rif, P->interfaces ) {
      struct iface *iface = rif->iface;

      if (!iface) continue;
      if (rif->mode & IM_QUIET) continue;
      if (!(iface->flags & IF_UP)) continue;
      rif->triggered = P->rnd_count;

      rip_sendto( p, IPA_NONE, 0, rif );
    }
    P->tx_count++;
    P->rnd_count--;
  }

  DBG( "RIP: tick tock done\n" );
}
