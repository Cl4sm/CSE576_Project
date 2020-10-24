/* sc_economy_config_destroy
   Obliterate a poor innocent economy config that was doing nothing wrong. */

   sc_scoring_info *info, *temp;

   if(ec == NULL || *ec == NULL) return;

   /* Delete all of our registry entries. */
   info = (sc_scoring_info *)sc_registry_find_first((*ec)->registry, (*ec)->registryclass,
                                                    SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
   while(info != NULL) {
      temp = info;
      info = (sc_scoring_info *)sc_registry_find_next((*ec)->registry, (*ec)->registryclass, info->ident,
                                                      SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
      sc_registry_del_by_int((*ec)->registry, temp->ident);
      sc_scoring_info_free(&temp);
   }

   /* And delete ourself. */
   free(*ec);
   *ec = NULL;

}
