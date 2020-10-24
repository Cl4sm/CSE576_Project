/* sc_weapon_config_create
   Allocate space and set defaults on a new sc_weapon_config struct. */

   sc_weapon_config *wc;
   const char *filename;

   assert(c != NULL && c->registry != NULL);

   wc = (sc_weapon_config *)malloc(sizeof(sc_weapon_config));
   if(wc == NULL) return(NULL);

   /* Default settings for weapon config. */
   wc->armslevel     = SC_ARMS_LEVEL_DEF;
   wc->bombiconsize  = SC_WEAPON_BOMB_ICON_DEF;
   wc->tunneling     = false;
   wc->scaling       = 1.0;
   wc->tracepaths    = false;
   wc->uselessitems  = true;

   /* Get a class ID for this weapon config. */
   wc->registryclass = sc_registry_get_new_class_id(c->registry);
   wc->registry      = c->registry;

   /* Read in the weapon info list */
   filename = SC_GLOBAL_DIR "/" SC_WEAPON_FILE;
   if(!sc_addconf_append_file(SC_ADDCONF_WEAPONS, filename, wc) ||
      sc_weapon_count(wc, SC_WEAPON_LIMIT_INDIRECT) <= 0) {
      /* This is the root weapon list...  Die! */
      free(wc);
      return(NULL);
   }

   return(wc);

}
