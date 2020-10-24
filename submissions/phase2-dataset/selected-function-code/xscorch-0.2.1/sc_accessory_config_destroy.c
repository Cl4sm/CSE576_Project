/* sc_accessory_config_destroy
   Invalidate memory used by an accessory config struct. */

   sc_accessory_info *info, *temp;

   if(ac == NULL || *ac == NULL) return;

   /* Delete all of our registry entries. */
   info = (sc_accessory_info *)sc_registry_find_first((*ac)->registry, (*ac)->registryclass,
                                                      SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
   while(info != NULL) {
      temp = info;
      info = (sc_accessory_info *)sc_registry_find_next((*ac)->registry, (*ac)->registryclass, info->ident,
                                                        SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
      sc_registry_del_by_int((*ac)->registry, temp->ident);
      sc_accessory_info_free(&temp);
   }

   /* And delete ourself. */
   free(*ac);
   *ac = NULL;

}
