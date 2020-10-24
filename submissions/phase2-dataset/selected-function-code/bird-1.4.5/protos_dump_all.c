protos_dump_all(void)
{
  struct proto *p;
  struct announce_hook *a;

  debug("Protocols:\n");

  WALK_LIST(p, active_proto_list)
    {
      debug("  protocol %s state %s/%s\n", p->name,
	    p_states[p->proto_state], c_states[p->core_state]);
      for (a = p->ahooks; a; a = a->next)
	{
	  debug("\tTABLE %s\n", a->table->name);
	  if (a->in_filter)
	    debug("\tInput filter: %s\n", filter_name(a->in_filter));
	  if (a->out_filter != FILTER_REJECT)
	    debug("\tOutput filter: %s\n", filter_name(a->out_filter));
	}
      if (p->disabled)
	debug("\tDISABLED\n");
      else if (p->proto->dump)
	p->proto->dump(p);
    }
  WALK_LIST(p, inactive_proto_list)
    debug("  inactive %s: state %s/%s\n", p->name, p_states[p->proto_state], c_states[p->core_state]);
  WALK_LIST(p, initial_proto_list)
    debug("  initial %s\n", p->name);
  WALK_LIST(p, flush_proto_list)
    debug("  flushing %s\n", p->name);
}
