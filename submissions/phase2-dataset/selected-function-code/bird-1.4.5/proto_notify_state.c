void
proto_notify_state(struct proto *p, unsigned ps)
{
  unsigned ops = p->proto_state;
  unsigned cs = p->core_state;
  unsigned es = p->export_state;

  DBG("%s reporting state transition %s/%s -> */%s\n", p->name, c_states[cs], p_states[ops], p_states[ps]);
  if (ops == ps)
    return;

  p->proto_state = ps;
  p->last_state_change = now;

  switch (ps)
    {
    case PS_START:
      ASSERT(ops == PS_DOWN || ops == PS_UP);
      ASSERT(cs == FS_HUNGRY || cs == FS_HAPPY);

      if (es != ES_DOWN)
	proto_want_export_down(p);
      break;

    case PS_UP:
      ASSERT(ops == PS_DOWN || ops == PS_START);
      ASSERT(cs == FS_HUNGRY || cs == FS_HAPPY);
      ASSERT(es == ES_DOWN);

      if (cs == FS_HUNGRY)
	proto_want_core_up(p);
      if (!p->gr_wait)
	proto_want_export_up(p);
      break;

    case PS_STOP:
      ASSERT(ops == PS_START || ops == PS_UP);

      p->down_sched = 0;

      if (es != ES_DOWN)
	proto_want_export_down(p);
      if (cs == FS_HAPPY)
	proto_want_core_down(p);
      proto_falling_down(p);
      break;

    case PS_DOWN:
      p->down_code = 0;
      p->down_sched = 0;

      if (es != ES_DOWN)
	proto_want_export_down(p);
      if (cs == FS_HAPPY)
	proto_want_core_down(p);
      if (ops != PS_STOP)
	proto_falling_down(p);

      neigh_prune(); // FIXME convert neighbors to resource?
      rfree(p->pool);
      p->pool = NULL;

      if (cs == FS_HUNGRY)		/* Shutdown finished */
	{
	  proto_log_state_change(p);
	  proto_fell_down(p);
	  return;			/* The protocol might have ceased to exist */
	}
      break;

    default:
      bug("%s: Invalid state %d", p->name, ps);
    }

  proto_log_state_change(p);
}
