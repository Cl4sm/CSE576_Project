void sc_economy_init(sc_economy_config *ec) {
/* sc_economy_init
   Initialize the economics for start of each game. */

   const sc_scoring_info *info;
   double ratio;

   assert(ec != NULL);

   /* If no scoring selected, use the default. */
   if(strnlenn(ec->scoringname, SC_ECONOMY_MAX_NAME_LEN)) {
      info = sc_scoring_lookup_by_name(ec, ec->scoringname);
   } else {
      info = (sc_scoring_info *)sc_registry_find_first(ec->registry, ec->registryclass,
                                                       SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
      strcopyb(ec->scoringname, info->name, SC_ECONOMY_MAX_NAME_LEN);
   }

   /* We *must* have have a scoring method.
      Should be impossible to trigger though. */
   assert(info != NULL);

   /* Scale by initial cash (for fairness)? */
   if(info->fixed) {
      ec->damagebonus   = info->damagebonus;
      ec->damageloss    = info->damageloss;
      ec->deathloss     = info->deathloss;
      ec->killbonus     = info->killbonus;
      ec->suicideloss   = info->suicideloss;
      ec->survivalbonus = info->survivalbonus;
   } else {
      ratio = ec->initialcash / SC_ECONOMY_DEF_CASH;
      ec->damagebonus   = info->damagebonus   * ratio;
      ec->damageloss    = info->damageloss    * ratio;
      ec->deathloss     = info->deathloss     * ratio;
      ec->killbonus     = info->killbonus     * ratio;
      ec->suicideloss   = info->suicideloss   * ratio;
      ec->survivalbonus = info->survivalbonus * ratio;
   }

   /* Start at the default interest rate. */
   ec->currentinterest  = ec->interestrate;

}
