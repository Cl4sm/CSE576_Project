void sc_weapon_config_destroy(sc_weapon_config **wc) {
/* sc_weapon_config_destroy
   Invalidate memory used by a weapon config struct. */

   sc_weapon_info *info, *temp;

   if(wc == NULL || *wc == NULL) return;

   /* Delete all of our registry entries. */
   info = (sc_weapon_info *)sc_registry_find_first((*wc)->registry, (*wc)->registryclass,
                                                   SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
   while(info != NULL) {
      temp = info;
      info = (sc_weapon_info *)sc_registry_find_next((*wc)->registry, (*wc)->registryclass, info->ident,
                                                     SC_REGISTRY_FORWARD, SC_REGISTRY_TEST_NULL, 0);
      sc_registry_del_by_int((*wc)->registry, temp->ident);
      sc_weapon_info_free(&temp);
   }

   /* And delete ourself. */
   free(*wc);
   *wc = NULL;

}
