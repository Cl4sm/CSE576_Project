int sc_config_file_init(sc_config *c) {
/* sc_config_file_init
   Initialise a registry database.  This function only initialises
   the registry structure with the classes and variables defined
   above; it does not (yet) read the configuration file. */

   char buf[REG_BUFFER];   /* Random buffer */
   reg *r;                 /* New registry DB */
   int i;                  /* Player iterator */

   /* Make sure the local directory exists. */
   if(!_sc_config_make_directory()) return(false);

   /* Figure out the filename for the conf file, and allocate the
      new registry database now. */
   sbprintf(buf, sizeof(buf), "%s/%s/%s", getenv("HOME"), SC_LOCAL_CONFIG_DIR, SC_LOCAL_CONFIGURATION);
   c->cfreg = reg_new(buf);
   if(c->cfreg == NULL) return(false);

   /* Setup the class listing */
   r = c->cfreg;
   reg_class_register_list(r, _reg_class_dataes);

   /* Register required toplevel variables, by setting a 
      default type for the variables. */
   reg_class_register_vars(r, REG_TOP, _reg_top_class);
   for(i = 0; _reg_top_class[i].name != NULL; ++i) {
      reg_set_block(r, NULL, _reg_top_class[i].name, _reg_top_class[i].klass);
   }

   /* Setup the players class, which has to be constructed dynamically
      (because we don't know the number of players available). */   
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      sbprintf(buf, sizeof(buf), "player_%d", i + 1);
      reg_class_register_var(r, "players_class", buf, REG_BLOCK, "player_class");
      sbprintf(buf, sizeof(buf), "players/player_%d", i + 1);
      reg_set_block(r, NULL, buf, "player_class");
   }

   /* Success */   
   return(true);

}
