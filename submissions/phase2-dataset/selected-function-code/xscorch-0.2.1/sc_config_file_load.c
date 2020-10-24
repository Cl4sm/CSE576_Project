/* sc_config_file_load
   Load the registry from user's config file. */

   int version;            /* Version of the config file */
   char buf[REG_BUFFER];   /* A temporary buffer */
   reg *r;                 /* Registry database */
   reg_var *player;        /* Player structure */
   int i;                  /* Player iterator */

   /* Load the registry */
   r = c->cfreg;
   if(!reg_load(r)) {
      printf("Didn't find a configuration file, will use game defaults.\n");
      return(true);
   }

   /* Find out what version we're playing with */
   version = 0;
   reg_get_integer(r, NULL, "version", &version);
   if(version < SC_CONFIG_VERSION) {
      fprintf(stderr, "config_file_load: failed to load registry version %d\n", SC_CONFIG_VERSION);
      fprintf(stderr, "config_file_load: trying to load using older reader.\n");
      fprintf(stderr, "config_file_load: please update your config file by resaving it.\n");
      return(sc_old_config_file_load(c));
   }

   /* Get the file datum */
   reg_get_string(r, NULL, "files/accessoryDefs", c->accessory_file, SC_FILENAME_LENGTH);
   reg_get_string(r, NULL, "files/tankProfiles",  c->profile_file,   SC_FILENAME_LENGTH);
   reg_get_string(r, NULL, "files/scoringDefs",   c->scoring_file,   SC_FILENAME_LENGTH);
   reg_get_string(r, NULL, "files/weaponDefs",    c->weapon_file,    SC_FILENAME_LENGTH);

   /* Get the font datum */
   reg_get_string(r, NULL, "fonts/fixedFont",       c->fixed_font,        SC_FONT_LENGTH);
   reg_get_string(r, NULL, "fonts/italicFixedFont", c->italic_fixed_font, SC_FONT_LENGTH);
   reg_get_string(r, NULL, "fonts/boldFixedFont",   c->bold_fixed_font,   SC_FONT_LENGTH);

   /* Load the auxiliary files here */
   if(*c->accessory_file != '\0' && !sc_addconf_append_file(SC_ADDCONF_ACCESSORIES, c->accessory_file, c->accessories)) {
      fprintf(stderr, "Failed to load additional accessories data from \"%s\"\n", c->accessory_file);
   } /* Loading accessories... */
   if(*c->profile_file != '\0' && !sc_tank_profile_add(&c->tanks, c->profile_file)) {
      fprintf(stderr, "Failed to load additional tank profile data from \"%s\"\n", c->profile_file);
   } /* Loading tank profiles... */
   if(*c->scoring_file != '\0' && !sc_addconf_append_file(SC_ADDCONF_SCORINGS, c->scoring_file, c->economics)) {
      fprintf(stderr, "Failed to load additional economy scoring data from \"%s\"\n", c->scoring_file);
   } /* Loading economy scoring... */
   if(*c->weapon_file != '\0' && !sc_addconf_append_file(SC_ADDCONF_WEAPONS, c->weapon_file, c->weapons)) {
      fprintf(stderr, "Failed to load additional weapons data from \"%s\"\n", c->weapon_file);
   } /* Loading weapons... */

   /* Load player data */
   reg_get_integer(r, NULL, "players/numPlayers", &c->numplayers);
   reg_get_integer(r, NULL, "players/numRounds",  &c->numrounds);
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      sbprintf(buf, sizeof(buf), "players/player_%d", i + 1);
      player = reg_get_var(r, NULL, buf);
      c->players[i]->aitype = reg_get_integer_from_values(r, player, "playerType", c->players[i]->aitype, sc_ai_names(), sc_ai_types());
      reg_get_string(r, player, "playerName", c->players[i]->name, SC_PLAYER_NAME_LENGTH);
   }

   /* Load basic options */   
   c->options.mode  = reg_get_integer_from_values(r, NULL, "options/mode",     c->options.mode,  sc_config_mode_names(),  sc_config_mode_types());
   c->options.team  = reg_get_integer_from_values(r, NULL, "options/teams",    c->options.team,  sc_config_team_names(),  sc_config_team_types());
   c->options.order = reg_get_integer_from_values(r, NULL, "options/order",    c->options.order, sc_config_order_names(), sc_config_order_types());
   c->options.talk  = reg_get_integer_from_values(r, NULL, "options/talkMode", c->options.talk,  sc_config_talk_names(),  sc_config_talk_types());
   reg_get_integer(r, NULL, "options/talkProbability", &c->options.talkprob);
   reg_get_boolean(r, NULL, "options/extendedStatus",  &c->options.extstatus);
   reg_get_boolean(r, NULL, "options/tooltips",        &c->options.tooltips);
   reg_get_boolean(r, NULL, "options/interleave",      &c->options.interleave);

   /* Load graphics settings */
   reg_get_integer(r, NULL, "graphics/fieldWidth",    &c->fieldwidth);
   reg_get_integer(r, NULL, "graphics/fieldHeight",   &c->fieldheight);
   reg_get_integer(r, NULL, "graphics/maxHeight",     &c->maxheight);
   reg_get_boolean(r, NULL, "graphics/dithering",     &c->graphics.gfxdither);
   reg_get_boolean(r, NULL, "graphics/animation",     &c->graphics.gfxanimate);
   reg_get_boolean(r, NULL, "graphics/graphicsFast",  &c->graphics.gfxfast);
   reg_get_boolean(r, NULL, "graphics/computersFast", &c->graphics.gfxcompfast);
   sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));

   /* Load economy options */   
   reg_get_doublev(r, NULL, "economics/interestRate",        &c->economics->interestrate);
   reg_get_boolean(r, NULL, "economics/dynamicInterest",     &c->economics->dynamicinterest);
   reg_get_integer(r, NULL, "economics/initialCash",         &c->economics->initialcash);
   reg_get_boolean(r, NULL, "economics/computersBuy",        &c->economics->computersbuy);
   reg_get_boolean(r, NULL, "economics/computersAggressive", &c->economics->computersaggressive);
   reg_get_boolean(r, NULL, "economics/freeMarket",          &c->economics->freemarket);
   reg_get_boolean(r, NULL, "economics/lottery",             &c->economics->lottery);
   reg_get_string( r, NULL, "economics/scoring",              c->economics->scoringname, SC_ECONOMY_MAX_NAME_LEN);

   /* Load physics options */   
   reg_get_doublev(r, NULL, "physics/airViscosity",  &c->physics->airviscosity);
   reg_get_doublev(r, NULL, "physics/gravity",       &c->physics->gravity);
   reg_get_doublev(r, NULL, "physics/tunnelDamping", &c->physics->damping);
   reg_get_doublev(r, NULL, "physics/maxWindSpeed",  &c->physics->maxwind);
   reg_get_boolean(r, NULL, "physics/dynamicWind",   &c->physics->dynamicwind);
   reg_get_integer(r, NULL, "physics/suspendDirt",   &c->physics->suspenddirt);
   reg_get_integer(r, NULL, "physics/tanksFall",     &c->physics->tanksfall);
   reg_get_integer(r, NULL, "physics/bordersExtend", &c->physics->bordersextend);
   c->physics->walls = reg_get_integer_from_values(r, NULL, "physics/walls", c->physics->walls, sc_physics_wall_names(), sc_physics_wall_types());

   /* Load weapons options */
   reg_get_integer(r, NULL, "weapons/armsLevel",    &c->weapons->armslevel);
   reg_get_integer(r, NULL, "weapons/bombIconSize", &c->weapons->bombiconsize);
   reg_get_boolean(r, NULL, "weapons/tunneling",    &c->weapons->tunneling);
   reg_get_doublev(r, NULL, "weapons/scaling",      &c->weapons->scaling);
   reg_get_boolean(r, NULL, "weapons/tracePaths",   &c->weapons->tracepaths);
   reg_get_boolean(r, NULL, "weapons/uselessItems", &c->weapons->uselessitems);

   /* Load AI Controller options */
   reg_get_boolean(r, NULL, "aicontroller/humanTargets", &c->aicontrol->humantargets);
   reg_get_boolean(r, NULL, "aicontroller/allowOffsets", &c->aicontrol->allowoffsets);
   reg_get_boolean(r, NULL, "aicontroller/alwaysOffset", &c->aicontrol->alwaysoffset);
   reg_get_boolean(r, NULL, "aicontroller/enableScan",   &c->aicontrol->enablescan);
   reg_get_boolean(r, NULL, "aicontroller/noBudget",     &c->aicontrol->nobudget);

   /* Load sound settings */
   reg_get_boolean(r, NULL, "sound/enableSound", &c->enablesound);
   reg_get_boolean(r, NULL, "sound/useHQMixer",  &c->usehqmixer);

   /* Load landscaping options */   
   c->land->sky       = reg_get_integer_from_values(r, NULL, "landscape/sky",       c->land->sky,       sc_land_sky_names(),       sc_land_sky_types());
   reg_get_boolean(r, NULL, "landscape/hostile",   &c->land->hostileenv);
   c->land->generator = reg_get_integer_from_values(r, NULL, "landscape/generator", c->land->generator, sc_land_generator_names(), sc_land_generator_types());
   reg_get_doublev(r, NULL, "landscape/bumpiness", &c->land->bumpiness);

   /* Success */
   return(true);

}
