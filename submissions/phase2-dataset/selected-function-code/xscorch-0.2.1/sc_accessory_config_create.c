/* sc_accessory_config_create
   Allocate space and set defaults on a new sc_accessory_config struct. */

   sc_accessory_config *ac;
   const char *filename;

   assert(c != NULL && c->registry != NULL);

   ac = (sc_accessory_config *)malloc(sizeof(sc_accessory_config));
   if(ac == NULL) return(NULL);

   /* Default settings for weapon config. */
   ac->armslevel = SC_ARMS_LEVEL_DEF;
   ac->uselessitems = true;

   /* Get a class ID for this accessory config. */
   ac->registryclass = sc_registry_get_new_class_id(c->registry);
   ac->registry      = c->registry;

   /* Read in the accessory info list */
   filename = SC_GLOBAL_DIR "/" SC_ACCESSORY_FILE;
   if(!sc_addconf_append_file(SC_ADDCONF_ACCESSORIES, filename, ac) ||
      sc_accessory_count(ac, SC_ACCESSORY_LIMIT_INDIRECT) <= 0) {
      /* This is the root accessory list...  Die! */
      free(ac);
      return(NULL);
   }

   return(ac);

}
