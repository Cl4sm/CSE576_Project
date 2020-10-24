proto_rethink_goal(struct proto *p)
{
  struct protocol *q;
  byte goal;

  if (p->reconfiguring && p->core_state == FS_HUNGRY && p->proto_state == PS_DOWN)
    {
      struct proto_config *nc = p->cf_new;
      DBG("%s has shut down for reconfiguration\n", p->name);
      p->cf->proto = NULL;
      config_del_obstacle(p->cf->global);
      rem_node(&p->n);
      rem_node(&p->glob_node);
      mb_free(p);
      if (!nc)
	return;
      p = proto_init(nc);
    }

  /* Determine what state we want to reach */
  if (p->disabled || p->reconfiguring)
    goal = PS_DOWN;
  else
    goal = PS_UP;

  q = p->proto;
  if (goal == PS_UP) 			/* Going up */
    {
      if (p->proto_state == PS_DOWN && p->core_state == FS_HUNGRY)
	{
	  DBG("Kicking %s up\n", p->name);
	  PD(p, "Starting");
	  proto_init_instance(p);
	  proto_notify_state(p, (q->start ? q->start(p) : PS_UP));
	}
    }
  else 					/* Going down */
    {
      if (p->proto_state == PS_START || p->proto_state == PS_UP)
	{
	  DBG("Kicking %s down\n", p->name);
	  PD(p, "Shutting down");
	  proto_notify_state(p, (q->shutdown ? q->shutdown(p) : PS_DOWN));
	}
    }
}
