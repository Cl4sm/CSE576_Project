sc_config *sc_config_new(int *argc, char ***argv) {
/* sc_config_new
   This function creates the configuration structure, initialises all
   values, and then calls the config file reader and options parser to 
   read in all the options.  It accepts the command line arguments as
   input.  */

   sc_config *c;           /* A new configuration structure */
   int i;                  /* General iterator */

   /* Initialise the random number generator */
   sys_randomize();

   /* Allocate the new configuration structure */
   c = (sc_config *)malloc(sizeof(sc_config));
   if(c == NULL) {
      fprintf(stderr, "config_new: failed to allocate config structure, this is critical.\n");
      return(NULL);
   }

   /* Setup config file parse tree */
   if(!sc_config_file_init(c) || c->cfreg == NULL) {
      fprintf(stderr, "config_new: failed to construct config file parser, this is critical.\n");
      return(NULL);
   }

   /* Clear out the user filenames */
   strcopyb(c->profile_file,     "", SC_FILENAME_LENGTH);
   strcopyb(c->weapon_file,      "", SC_FILENAME_LENGTH);
   strcopyb(c->accessory_file,   "", SC_FILENAME_LENGTH);

   /* Set default fontspecs */
   strcopyb(c->fixed_font,          "-adobe-courier-medium-r-*-*-*-120-*", SC_FONT_LENGTH);
   strcopyb(c->italic_fixed_font,   "-adobe-courier-medium-o-*-*-*-120-*", SC_FONT_LENGTH);
   strcopyb(c->bold_fixed_font,     "-adobe-courier-bold-r-*-*-*-120-*",   SC_FONT_LENGTH);

   /* Initialise screen size */
   c->fieldwidth = SC_DEF_FIELD_WIDTH;
   c->fieldheight= SC_DEF_FIELD_HEIGHT;
   c->maxheight  = SC_DEF_MAX_HEIGHT;

   /* Initialise local parameters/structures */
   c->insanity   = false;
   c->numrounds  = SC_DEF_ROUNDS;
   c->numplayers = SC_DEF_PLAYERS;
   c->window     = NULL;
   c->componly   = false;

   /* Set default configuration options */
   c->options.mode      = SC_CONFIG_MODE_SYNCHRONOUS;
   c->options.team      = SC_CONFIG_TEAM_NONE;
   c->options.order     = SC_CONFIG_ORDER_RANDOM;
   c->options.talk      = SC_CONFIG_TALK_ALL;
   c->options.talkprob  = SC_CONFIG_TALK_PROB_DEF;
   c->options.extstatus = true;
   c->options.tooltips  = true;
   c->options.interleave= false;

   /* Set the default graphics options */
   c->graphics.gfxdither   = true;
   c->graphics.gfxanimate  = true;
   c->graphics.gfxfast     = false;
   c->graphics.gfxcompfast = false;

   /* Setup AI controller */
   c->aicontrol = sc_ai_controller_new();
   if(c->aicontrol == NULL) {
      fprintf(stderr, "config_new: failed to build ai_controller.\n");
      return(NULL);
   }

   /* Initialise the physics model */   
   c->physics = sc_physics_new();
   if(c->physics == NULL) {
      fprintf(stderr, "config_new: failed to build physics.\n");
      return(NULL);
   }

   /* Create the new land structure (uninitialised) */
   /* WARNING: must be initialised AFTER physics model */
   c->land = sc_land_new(c->fieldwidth, c->fieldheight, sc_land_flags(c));
   if(c->land == NULL) {
      fprintf(stderr, "config_new: failed to build land.\n");
      return(NULL);
   }

   /* Allocate and initialize the game data registry (this is over 64KiB) */
   /* WARNING: must be initialized BEFORE any registry users
      (currently that means weapons and accessories storage) */
   c->registry = sc_registry_new();
   if(c->registry == NULL) {
      fprintf(stderr, "config_new: failed to build game data registry.\n");
      return(NULL);
   }

   /* Create the new lottery structure */
   c->lottery = sc_lottery_new();
   if(c->lottery == NULL) {
      fprintf(stderr, "config_new: failed to build lottery.\n");
      return(NULL);
   }

   /* Load default tank profiles */
   c->tanks = NULL;
   if(!sc_tank_profile_add(&c->tanks, SC_GLOBAL_DIR "/" SC_TANK_PROFILE_FILE) || c->tanks == NULL) {
      fprintf(stderr, "config_new: failed to build tanks_profile, or no tanks in def file.\n");
      return(NULL);
   }

   /* Create the players; clear the player order */
   /* WARNING: must be initialised AFTER tank profiles */
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      c->plorder[i] = NULL;
      c->players[i] = sc_player_new(i, c->tanks);
      if(c->players[i] == NULL) {
         fprintf(stderr, "config_new: failed to build player, %d'th hunk failed.\n", i);
         return(NULL);
      }
   }

   /* Initialise the game state structure */
   c->game = sc_game_new();
   if(c->game == NULL) {
      fprintf(stderr, "config_new: failed to build game.\n");
      return(NULL);
   }

   /* Initialise the color structure */   
   c->colors = sc_color_new();
   if(c->colors == NULL) {
      fprintf(stderr, "config_new: failed to build colors.\n");
      return(NULL);
   }

   /* Initialise the accessories configuration */
   c->accessories = sc_accessory_config_create(c);
   if(c->accessories == NULL) {
      fprintf(stderr, "config_new: failed to build accessory_config, or no accessories in def file.\n");
      return(NULL);
   }

   /* Initialise the economy and markets */
   c->economics = sc_economy_config_create(c);
   if(c->economics == NULL) {
      fprintf(stderr, "config_new: failed to build economy_config, or no scorings in def file.\n");
      return(NULL);
   }

   /* Initialise the weapons configuration */
   c->weapons = sc_weapon_config_create(c);
   if(c->weapons == NULL) {
      fprintf(stderr, "config_new: failed to build weapon_config, or no weapons in def file.\n");
      return(NULL);
   }

   /* Initialise sound controller */
   c->enablesound = true;
   c->usehqmixer = false;
   sc_sound_init();

   /* Initialise network connections? */
   #if USE_NETWORK
      c->client = NULL;
      c->server = NULL;
   #endif /* Network allowed? */

   /* Parse command-line and file options */
   if(!sc_config_file_load(c)) {
      fprintf(stderr, "config_new: critical error loading config file.\n");
      return(NULL);
   }
   if(sc_options_parse(c, *argc, *argv)) return(NULL);

   /* Start sound playback, if permitted */
   c->sound = sc_sound_new(c->enablesound, c->usehqmixer);

   /* Return the new configuration structure */
   return(c);

}
