/* sc_economy_config_create
   Allocate space and read in economy scoring definitions. */

   sc_economy_config *ec;
   const char *filename;

   ec = (sc_economy_config *)malloc(sizeof(sc_economy_config));
   if(ec == NULL) return(NULL);

   ec->registry        = c->registry;
   ec->interestrate    = SC_ECONOMY_DEF_INTEREST;
   ec->dynamicinterest = false;
   ec->initialcash     = SC_ECONOMY_DEF_CASH;
   ec->computersbuy    = true;
   ec->computersaggressive = false;
   ec->freemarket      = false;
   ec->lottery         = false;
   ec->scoringname[0]  = '\0';

   /* Get a class ID for this economy config. */
   ec->registryclass   = sc_registry_get_new_class_id(c->registry);
   ec->registry        = c->registry;

   /* Read in the scoring info list */
   filename = SC_GLOBAL_DIR "/" SC_SCORING_FILE;
   if(!sc_addconf_append_file(SC_ADDCONF_SCORINGS, filename, ec)) {
      /* This is the root scoring types list...  Die! */
      free(ec);
      return(NULL);
   }

   return(ec);

}
