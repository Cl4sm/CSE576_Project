void
protos_commit(struct config *new, struct config *old, int force_reconfig, int type)
{
  struct proto_config *oc, *nc;
  struct proto *p, *n;
  struct symbol *sym;

  DBG("protos_commit:\n");
  if (old)
    {
      WALK_LIST(oc, old->protos)
	{
	  p = oc->proto;
	  sym = cf_find_symbol(oc->name);
	  if (sym && sym->class == SYM_PROTO && !new->shutdown)
	    {
	      /* Found match, let's check if we can smoothly switch to new configuration */
	      /* No need to check description */
	      nc = sym->def;
	      nc->proto = p;

	      /* We will try to reconfigure protocol p */
	      if (! force_reconfig && proto_reconfigure(p, oc, nc, type))
		continue;

	      /* Unsuccessful, we will restart it */
	      if (!p->disabled && !nc->disabled)
		log(L_INFO "Restarting protocol %s", p->name);
	      else if (p->disabled && !nc->disabled)
		log(L_INFO "Enabling protocol %s", p->name);
	      else if (!p->disabled && nc->disabled)
		log(L_INFO "Disabling protocol %s", p->name);

	      p->down_code = nc->disabled ? PDC_CF_DISABLE : PDC_CF_RESTART;
	      p->cf_new = nc;
	    }
	  else if (!new->shutdown)
	    {
	      log(L_INFO "Removing protocol %s", p->name);
	      p->down_code = PDC_CF_REMOVE;
	      p->cf_new = NULL;
	    }
	  else /* global shutdown */
	    {
	      p->down_code = PDC_CMD_SHUTDOWN;
	      p->cf_new = NULL;
	    }

	  p->reconfiguring = 1;
	  config_add_obstacle(old);
	  proto_rethink_goal(p);
	}
    }

  WALK_LIST(nc, new->protos)
    if (!nc->proto)
      {
	if (old)		/* Not a first-time configuration */
	  log(L_INFO "Adding protocol %s", nc->name);
	proto_init(nc);
      }
  DBG("\tdone\n");

  DBG("Protocol start\n");

  /* Start device protocol first */
  if (initial_device_proto)
  {
    proto_rethink_goal(initial_device_proto);
    initial_device_proto = NULL;
  }

  /* Determine router ID for the first time - it has to be here and not in
     global_commit() because it is postponed after start of device protocol */
  if (!config->router_id)
    {
      config->router_id = if_choose_router_id(config->router_id_from, 0);
      if (!config->router_id)
	die("Cannot determine router ID, please configure it manually");
    }

  /* Start all other protocols */
  WALK_LIST_DELSAFE(p, n, initial_proto_list)
    proto_rethink_goal(p);
}
