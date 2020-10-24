/* sc_old_config_file_load */

   int version;            /* Version of the config file */
   char buf[0x1000];       /* A temporary buffer */
   reg_var *player;        /* Player structure */
   reg *r;                 /* New registry DB */
   int i;                  /* Player iterator */

   /* Sanity checks */
   if(c == NULL || c->cfreg == NULL) return(false);

   /* Setup the class listing */
   r = reg_new(c->cfreg->filename);
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

   /* Load the registry */
   if(!reg_load(r)) {
      return(true);
   }

   /* Find out what version we're playing with */
   version = 0;
   reg_get_integer(r, NULL, "version", &version);
   
   /* Get the file datum */
   reg_get_string(r, NULL, "files/accessoryDefs", c->accessory_file, SC_FILENAME_LENGTH);
   reg_get_string(r, NULL, "files/tankProfiles", c->profile_file, SC_FILENAME_LENGTH);
   reg_get_string(r, NULL, "files/weaponDefs", c->weapon_file, SC_FILENAME_LENGTH);
   
   /* Load the auxiliary files here */
   if(*c->accessory_file != '\0' && !sc_addconf_append_file(SC_ADDCONF_ACCESSORIES, c->accessory_file, c->accessories)) {
      fprintf(stderr, "Failed to load additional accessories data from \"%s\"\n", c->accessory_file);
   } /* Loading accessories... */
   if(*c->profile_file != '\0' && !sc_tank_profile_add(&c->tanks, c->profile_file)) {
      fprintf(stderr, "Failed to load additional tank profile data from \"%s\"\n", c->profile_file);
   } /* Loading tank profiles... */
   if(*c->weapon_file != '\0' && !sc_addconf_append_file(SC_ADDCONF_WEAPONS, c->weapon_file, c->weapons)) {
      fprintf(stderr, "Failed to load additional weapons data from \"%s\"\n", c->weapon_file);
   } /* Loading weapons... */
   
   /* Load player data */
   reg_get_integer(r, NULL, "players/numPlayers", &c->numplayers);
   reg_get_integer(r, NULL, "players/numRounds", &c->numrounds);
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      sbprintf(buf, sizeof(buf), "players/player_%d", i + 1);
      player = reg_get_var(r, NULL, buf);
      c->players[i]->aitype = reg_get_integer_from_values(r, player, "playerType", c->players[i]->aitype, sc_ai_names(), sc_ai_types());
      reg_get_string(r, player, "playerName", c->players[i]->name, SC_PLAYER_NAME_LENGTH);
   }

   /* Load basic options */   
   c->options.mode = reg_get_integer_from_values(r, NULL, "options/mode", c->options.mode, sc_config_mode_names(), sc_config_mode_types());
   c->options.team = reg_get_integer_from_values(r, NULL, "options/teams", c->options.team, sc_config_team_names(), sc_config_team_types());
   c->options.order = reg_get_integer_from_values(r, NULL, "options/order", c->options.order, sc_config_order_names(), sc_config_order_types());
   c->options.talk = reg_get_integer_from_values(r, NULL, "options/talkMode", c->options.talk, sc_config_talk_names(), sc_config_talk_types());
   reg_get_integer(r, NULL, "options/talkProbability", &c->options.talkprob);

   /* Load graphics settings */
   reg_get_integer(r, NULL, "graphics/fieldWidth", &c->fieldwidth);
   reg_get_integer(r, NULL, "graphics/fieldHeight", &c->fieldheight);
   reg_get_integer(r, NULL, "graphics/maxHeight", &c->maxheight);
   reg_get_integer(r, NULL, "graphics/dithering", (int *)&c->graphics.gfxdither);
   reg_get_integer(r, NULL, "graphics/animation", (int *)&c->graphics.gfxanimate);
   reg_get_integer(r, NULL, "graphics/graphicsFast", (int *)&c->graphics.gfxfast);
   reg_get_integer(r, NULL, "graphics/computersFast", (int *)&c->graphics.gfxcompfast);
   sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));

   /* Load economy options */   
   reg_get_doublev(r, NULL, "economics/interestRate", &c->economics->interestrate);
   reg_get_integer(r, NULL, "economics/dynamicInterest", (int *)&c->economics->dynamicinterest);
   reg_get_integer(r, NULL, "economics/initialCash", &c->economics->initialcash);
   reg_get_integer(r, NULL, "economics/computersBuy", (int *)&c->economics->computersbuy);
   reg_get_integer(r, NULL, "economics/computersAggressive", (int *)&c->economics->computersaggressive);
   reg_get_integer(r, NULL, "economics/freeMarket", (int *)&c->economics->freemarket);
   reg_get_integer(r, NULL, "economics/lottery", (int *)&c->economics->lottery);
   /* TEMP - This is obsolete; ->scoring no longer exists. -JL
    * c->economy->scoring = reg_get_integer_from_values(r, NULL, "economics/scoring", c->economics->scoring, sc_economy_scoring_names(), sc_economy_scoring_types());
    */

   /* Load physics options */   
   reg_get_doublev(r, NULL, "physics/airViscosity", &c->physics->airviscosity);
   reg_get_doublev(r, NULL, "physics/gravity", &c->physics->gravity);
   reg_get_doublev(r, NULL, "physics/maxWindSpeed", &c->physics->maxwind);
   reg_get_integer(r, NULL, "physics/dynamicWind", (int *)&c->physics->dynamicwind);
   reg_get_integer(r, NULL, "physics/suspendDirt", &c->physics->suspenddirt);
   reg_get_integer(r, NULL, "physics/tanksFall", &c->physics->tanksfall);
   reg_get_integer(r, NULL, "physics/bordersExtend", &c->physics->bordersextend);
   c->physics->walls = reg_get_integer_from_values(r, NULL, "physics/walls", c->physics->walls, sc_physics_wall_names(), sc_physics_wall_types());

   /* Load weapons options */
   reg_get_integer(r, NULL, "weapons/armsLevel", &c->weapons->armslevel);
   reg_get_integer(r, NULL, "weapons/bombIconSize", &c->weapons->bombiconsize);
   reg_get_integer(r, NULL, "weapons/tunneling", (int *)&c->weapons->tunneling);
   reg_get_doublev(r, NULL, "weapons/scaling", &c->weapons->scaling);
   reg_get_integer(r, NULL, "weapons/tracePaths", (int *)&c->weapons->tracepaths);
   reg_get_integer(r, NULL, "weapons/uselessItems", (int *)&c->weapons->uselessitems);

   /* Load AI Controller options */
   reg_get_integer(r, NULL, "aicontroller/humanTargets", (int *)&c->aicontrol->humantargets);
   reg_get_integer(r, NULL, "aicontroller/allowOffsets", (int *)&c->aicontrol->allowoffsets);
   reg_get_integer(r, NULL, "aicontroller/noBudget", (int *)&c->aicontrol->nobudget);

   /* Load sound settings */
   reg_get_integer(r, NULL, "sound/enableSound", (int *)&c->enablesound);
   reg_get_integer(r, NULL, "sound/useHQMixer",  (int *)&c->usehqmixer);

   /* Load landscaping options */   
   c->land->sky = reg_get_integer_from_values(r, NULL, "landscape/sky", c->land->sky, sc_land_sky_names(), sc_land_sky_types());
   reg_get_integer(r, NULL, "landscape/hostile", (int *)&c->land->hostileenv);
   c->land->generator = reg_get_integer_from_values(r, NULL, "landscape/generator", c->land->generator, sc_land_generator_names(), sc_land_generator_types());
   reg_get_doublev(r, NULL, "landscape/bumpiness", &c->land->bumpiness);
   
   /* Release the registry */
   reg_free(&r);

   /* Success */
   return(true);

}
