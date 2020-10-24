                                        sc_economy_setup_data_gtk *setup) {

   sc_scoring_info *info;

   setup->ec->interestrate    = setup->interestrate;
   setup->ec->dynamicinterest = setup->dynamicinterest;
   setup->ec->initialcash     = setup->initialcash;
   setup->ec->computersbuy    = setup->computersbuy;
   setup->ec->computersaggressive = setup->computersaggressive;
   setup->ec->freemarket      = setup->freemarket;
   setup->ec->lottery         = setup->lottery;

   /* TEMP HACK - Well, this is ugly, maybe it should be done differently...
                  We could make it sane by reviving old interfaces in seconomy. -JL */

   info = sc_registry_find_first(setup->ec->registry, setup->ec->registryclass,
                                 SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
   for(; setup->economyidx > 0; --setup->economyidx)
      info = sc_registry_find_next(setup->ec->registry, setup->ec->registryclass, info->ident,
                                   SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);

   /* Set the new selected economy. */
   strcopyb(setup->ec->scoringname, info->name, SC_ECONOMY_MAX_NAME_LEN);

   #if USE_NETWORK
   if(SC_NETWORK_SERVER(setup->c)) sc_net_server_send_config(setup->c, setup->c->server);
   #endif

}
