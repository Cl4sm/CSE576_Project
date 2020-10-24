
   char buffer[SC_NET_BUFFER_SIZE];   /* For info messages */
   dword *p;            /* Pointer to packet data */

   /* Sanity checks */
   if(c == NULL || cli == NULL || packet == NULL) return(false);
   if(!sc_net_check_size(packet, SC_PACKET_CONFIG_SIZE, "cli_recv_config")) return(false);

   /* Prepare to read data from config packet */
   p = (dword *)packet->data;

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_OPTIONS, "cli_recv_config", "options marker")) return(false);
   c->options.mode         = ntohl(*p++);
   c->options.team         = ntohl(*p++);
   c->options.order        = ntohl(*p++);
   c->options.talk         = ntohl(*p++);
   c->options.talkprob     = ntohl(*p++);
   c->options.interleave   = ntohl(*p++);

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_GRAPHICS, "cli_recv_config", "graphics marker")) return(false);
   c->graphics.gfxdither   = ntohl(*p++);
   c->graphics.gfxanimate  = ntohl(*p++);
   c->graphics.gfxfast     = ntohl(*p++);
   c->graphics.gfxcompfast = ntohl(*p++);
   c->fieldwidth           = ntohl(*p++);
   c->fieldheight          = ntohl(*p++);
   c->maxheight            = ntohl(*p++);

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_ECONOMY, "cli_recv_config", "economy marker")) return(false);
   c->economics->interestrate        = ((double)ntohl(*p++)) * 100 / DWORD_MAX;
   c->economics->dynamicinterest     = ntohl(*p++);
   c->economics->initialcash         = ntohl(*p++);
   c->economics->computersbuy        = ntohl(*p++);
   c->economics->computersaggressive = ntohl(*p++);
   c->economics->freemarket          = ntohl(*p++);
   c->economics->lottery             = ntohl(*p++);
   { /* TEMP HACK - More of the same evil.  But I know the eventual solution... */
      sc_scoring_info *info = sc_scoring_lookup(c->economics, ntohl(*p++));
      assert(info != NULL);
      strcopyb(c->economics->scoringname, info->name, SC_ECONOMY_MAX_NAME_LEN);
   }

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_PHYSICS, "cli_recv_config", "physics marker")) return(false);
   c->physics->airviscosity  = ((double)ntohl(*p++)) * SC_PHYSICS_VISCOUS_MAX / DWORD_MAX;
   c->physics->gravity       = ((double)ntohl(*p++)) * SC_PHYSICS_GRAVITY_MAX / DWORD_MAX;
   c->physics->damping       = ((double)ntohl(*p++)) * SC_TRAJ_DAMPING_MAX / DWORD_MAX;
   c->physics->maxwind       = ((double)ntohl(*p++)) * SC_PHYSICS_WIND_MAX / DWORD_MAX;
   c->physics->dynamicwind   = ntohl(*p++);
   c->physics->suspenddirt   = ntohl(*p++);
   c->physics->tanksfall     = ntohl(*p++);
   c->physics->bordersextend = ntohl(*p++);
   c->physics->walls         = ntohl(*p++);

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_WEAPONS, "cli_recv_config", "weapons marker")) return(false);
   c->weapons->armslevel    = ntohl(*p++);
   c->weapons->bombiconsize = ntohl(*p++);
   c->weapons->tunneling    = ntohl(*p++);
   c->weapons->scaling      = ((double)ntohl(*p++)) * SC_WEAPON_SCALING_MAX / DWORD_MAX;
   c->weapons->tracepaths   = ntohl(*p++);
   c->weapons->uselessitems = ntohl(*p++);

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_LAND, "cli_recv_config", "land marker")) return(false);
   c->land->sky        = ntohl(*p++);
   c->land->hostileenv = ntohl(*p++);
   c->land->generator  = ntohl(*p++);
   c->land->bumpiness  = ((double)ntohl(*p++)) * SC_LAND_BUMPINESS_MAX / DWORD_MAX;

   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_AIC, "cli_recv_config", "aic marker")) return(false);
   c->aicontrol->humantargets = ntohl(*p++);
   c->aicontrol->allowoffsets = ntohl(*p++);
   c->aicontrol->alwaysoffset = ntohl(*p++);
   c->aicontrol->enablescan   = ntohl(*p++);
   c->aicontrol->nobudget     = ntohl(*p++);

   /* Pick up runtime data */
   if(!sc_net_check_param(ntohl(*p++), SC_NET_CONFIG_RUNTIME, "cli_recv_config", "runtime marker")) return(false);
   sbprintf(buffer, sizeof(buffer), "Random seed is %08x", ntohl(*p));
   sc_net_set_info("cli_recv_config", buffer);
   game_randomize(ntohl(*p++));
   c->numplayers = ntohl(*p++);

   /*  B E G I N   C O D E   B A S E D   O N   G R A P H I C S   S E T U P  */

   /* Sloppily reconfigure everything on-the-fly */
   /* Colormap MUST be recalculated before land is regenerated! */
   sc_color_gradient_init(c, c->colors);

   /* Attempt to rebuild land */   
   sc_land_setup(c->land, c->fieldwidth, c->fieldheight, sc_land_flags(c));
   sc_land_generate(c, c->land);

   /* Resize window and redraw everything */
   sc_window_resize(c->window);

   /*  E N D   C O D E   B A S E D   O N   G R A P H I C S   S E T U P  */

   /* We are done */
   sc_net_set_info("cli_recv_config", "config structure received");
   SC_CONN_CLEAR_FLAGS(cli->server, SC_CONN_NEED_CONFIG);

   /* Return with success */
   return(true);

}
