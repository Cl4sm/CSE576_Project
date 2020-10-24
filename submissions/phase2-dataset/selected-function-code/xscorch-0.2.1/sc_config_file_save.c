/* sc_config_file_save
   Save the user options. */

   char buf[REG_BUFFER];   /* A temporary buffer */
   reg *r;                 /* Registry database */
   reg_var *player;        /* Player variable */
   int i;                  /* Player iterator */

   /* Get the registry DB */
   r = c->cfreg;

   /* Save the proper version number. */
   reg_set_integer(r, NULL, "version", SC_CONFIG_VERSION);

   /* Save the file datum */
   reg_set_string(r, NULL, "files/accessoryDefs", c->accessory_file);
   reg_set_string(r, NULL, "files/tankProfiles",  c->profile_file);
   reg_set_string(r, NULL, "files/scoringDefs",   c->scoring_file);
   reg_set_string(r, NULL, "files/weaponDefs",    c->weapon_file);

   /* Save the font datum */
   reg_set_string(r, NULL, "fonts/fixedFont",       c->fixed_font);
   reg_set_string(r, NULL, "fonts/italicFixedFont", c->italic_fixed_font);
   reg_set_string(r, NULL, "fonts/boldFixedFont",   c->bold_fixed_font);

   /* Save the current player data. */
   reg_set_integer(r, NULL, "players/numPlayers", c->numplayers);
   reg_set_integer(r, NULL, "players/numRounds",  c->numrounds);
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      sbprintf(buf, sizeof(buf), "players/player_%d", i + 1);
      player = reg_get_var(r, NULL, buf);
      reg_set_string_from_values(r, player, "playerType", c->players[i]->aitype, sc_ai_names(), sc_ai_types());
      reg_set_string(r, player, "playerName", c->players[i]->name);
   }

   /* Save the basic options */   
   reg_set_string_from_values(r, NULL, "options/mode",     c->options.mode,  sc_config_mode_names(),  sc_config_mode_types());
   reg_set_string_from_values(r, NULL, "options/teams",    c->options.team,  sc_config_team_names(),  sc_config_team_types());
   reg_set_string_from_values(r, NULL, "options/order",    c->options.order, sc_config_order_names(), sc_config_order_types());
   reg_set_string_from_values(r, NULL, "options/talkMode", c->options.talk,  sc_config_talk_names(),  sc_config_talk_types());
   reg_set_integer(r, NULL, "options/talkProbability", c->options.talkprob);
   reg_set_boolean(r, NULL, "options/extendedStatus",  c->options.extstatus);
   reg_set_boolean(r, NULL, "options/tooltips",        c->options.tooltips);
   reg_set_boolean(r, NULL, "options/interleave",      c->options.interleave);

   /* Save the graphics options */
   reg_set_integer(r, NULL, "graphics/fieldWidth",    c->fieldwidth);
   reg_set_integer(r, NULL, "graphics/fieldHeight",   c->fieldheight);
   reg_set_integer(r, NULL, "graphics/maxHeight",     c->maxheight);
   reg_set_boolean(r, NULL, "graphics/dithering",     c->graphics.gfxdither);
   reg_set_boolean(r, NULL, "graphics/animation",     c->graphics.gfxanimate);
   reg_set_boolean(r, NULL, "graphics/graphicsFast",  c->graphics.gfxfast);
   reg_set_boolean(r, NULL, "graphics/computersFast", c->graphics.gfxcompfast);

   /* Save the economics options */
   reg_set_doublev(r, NULL, "economics/interestRate",    c->economics->interestrate);
   reg_set_boolean(r, NULL, "economics/dynamicInterest", c->economics->dynamicinterest);
   reg_set_integer(r, NULL, "economics/initialCash",     c->economics->initialcash);
   reg_set_boolean(r, NULL, "economics/computersBuy",    c->economics->computersbuy);
   reg_set_boolean(r, NULL, "economics/computersAggressive", c->economics->computersaggressive);
   reg_set_boolean(r, NULL, "economics/freeMarket",      c->economics->freemarket);
   reg_set_boolean(r, NULL, "economics/lottery",         c->economics->lottery);
   reg_set_string( r, NULL, "economics/scoring",         c->economics->scoringname);

   /* Save the physics options */
   reg_set_doublev(r, NULL, "physics/airViscosity",  c->physics->airviscosity);
   reg_set_doublev(r, NULL, "physics/gravity",       c->physics->gravity);
   reg_set_doublev(r, NULL, "physics/tunnelDamping", c->physics->damping);
   reg_set_doublev(r, NULL, "physics/maxWindSpeed",  c->physics->maxwind);
   reg_set_boolean(r, NULL, "physics/dynamicWind",   c->physics->dynamicwind);
   reg_set_integer(r, NULL, "physics/suspendDirt",   c->physics->suspenddirt);
   reg_set_integer(r, NULL, "physics/tanksFall",     c->physics->tanksfall);
   reg_set_integer(r, NULL, "physics/bordersExtend", c->physics->bordersextend);
   reg_set_string_from_values(r, NULL, "physics/walls", c->physics->walls, sc_physics_wall_names(), sc_physics_wall_types());

   /* Save the weapons options */
   reg_set_integer(r, NULL, "weapons/armsLevel",    c->weapons->armslevel);
   reg_set_integer(r, NULL, "weapons/bombIconSize", c->weapons->bombiconsize);
   reg_set_boolean(r, NULL, "weapons/tunneling",    c->weapons->tunneling);
   reg_set_doublev(r, NULL, "weapons/scaling",      c->weapons->scaling);
   reg_set_boolean(r, NULL, "weapons/tracePaths",   c->weapons->tracepaths);
   reg_set_boolean(r, NULL, "weapons/uselessItems", c->weapons->uselessitems);

   /* Save AI controller options */
   reg_set_boolean(r, NULL, "aicontroller/humanTargets", c->aicontrol->humantargets);
   reg_set_boolean(r, NULL, "aicontroller/allowOffsets", c->aicontrol->allowoffsets);
   reg_set_boolean(r, NULL, "aicontroller/alwaysOffset", c->aicontrol->alwaysoffset);
   reg_set_boolean(r, NULL, "aicontroller/enableScan",   c->aicontrol->enablescan);
   reg_set_boolean(r, NULL, "aicontroller/noBudget",     c->aicontrol->nobudget);

   /* Save sound settings */   
   reg_set_boolean(r, NULL, "sound/enableSound", c->enablesound);
   reg_set_boolean(r, NULL, "sound/useHQMixer",  c->usehqmixer);

   /* Save landscaping options */
   reg_set_string_from_values(r, NULL, "landscape/sky",       c->land->sky,       sc_land_sky_names(),       sc_land_sky_types());
   reg_set_boolean(r, NULL, "landscape/hostile",   c->land->hostileenv);
   reg_set_string_from_values(r, NULL, "landscape/generator", c->land->generator, sc_land_generator_names(), sc_land_generator_types());
   reg_set_doublev(r, NULL, "landscape/bumpiness", c->land->bumpiness);

   /* Write everything to a file. */
   return(reg_save(r));

}
