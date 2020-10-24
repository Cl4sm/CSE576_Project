static void
proto_flush_loop(void *unused UNUSED)
{
  struct proto *p;

  if (! rt_prune_loop())
    {
      /* Rtable pruning is not finished */
      ev_schedule(proto_flush_event);
      return;
    }

  rt_prune_sources();

 again:
  WALK_LIST(p, flush_proto_list)
    if (p->flushing)
      {
	/* This will flush interfaces in the same manner
	   like rt_prune_all() flushes routes */
	if (p->proto == &proto_unix_iface)
	  if_flush_ifaces(p);

	DBG("Flushing protocol %s\n", p->name);
	p->flushing = 0;
	p->core_state = FS_HUNGRY;
	proto_relink(p);
	proto_log_state_change(p);
	if (p->proto_state == PS_DOWN)
	  proto_fell_down(p);
	goto again;
      }

  /* This round finished, perhaps there will be another one */
  flush_loop_state = 0;
  if (!EMPTY_LIST(flush_proto_list))
    proto_schedule_flush_loop();
}
