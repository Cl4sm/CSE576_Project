
   sc_packet config;    /* Data packet to transmit to server broadcast */
   sc_packet player;    /* player data to transmit to server broadcast */
   dword randseed;      /* Initial random seed for game */
   dword *p;            /* Pointer  into packet data */
   ubyte *pc;            /* Char ptr into packet data */
   int i;               /* Iterator through connections */
   int j;               /* Inner loop iterator */

   /* Sanity checks */
   if(c == NULL || svr == NULL) return(false);

   /* Obtain a new random seed */
   randseed = sys_rand();

   /*  B E G I N   C O N F I G   P A C K E T  */
   #define SC_PACKET_CONFIG_SIZE    (sizeof(dword) * 55)

   /* Construct the data area of config packet */
   if(!sc_net_packet_init(&config, SC_NET_SVR_CONFIG_DATA, SC_PACKET_CONFIG_SIZE)) return(false);
   p = (dword *)config.data;
   *p++ = htonl(SC_NET_CONFIG_OPTIONS);
   *p++ = htonl(c->options.mode);
   *p++ = htonl(c->options.team);
   *p++ = htonl(c->options.order);
   *p++ = htonl(c->options.talk);
   *p++ = htonl(c->options.talkprob);
   *p++ = htonl(c->options.interleave);

   *p++ = htonl(SC_NET_CONFIG_GRAPHICS);
   *p++ = htonl(c->graphics.gfxdither);
   *p++ = htonl(c->graphics.gfxanimate);
   *p++ = htonl(c->graphics.gfxfast);
   *p++ = htonl(c->graphics.gfxcompfast);
   *p++ = htonl(c->fieldwidth);
   *p++ = htonl(c->fieldheight);
   *p++ = htonl(c->maxheight);

   *p++ = htonl(SC_NET_CONFIG_ECONOMY);
   *p++ = htonl((dword)(c->economics->interestrate / 100 * DWORD_MAX));
   *p++ = htonl(c->economics->dynamicinterest);
   *p++ = htonl(c->economics->initialcash);
   *p++ = htonl(c->economics->computersbuy);
   *p++ = htonl(c->economics->computersaggressive);
   *p++ = htonl(c->economics->freemarket);
   *p++ = htonl(c->economics->lottery);
   { /* TEMP HACK - This is so evil I can't believe I'm doing it. */
      sc_scoring_info *info = sc_scoring_lookup_by_name(c->economics, c->economics->scoringname);
      assert(info != NULL);
      *p++ = htonl(info->ident);
   }

   *p++ = htonl(SC_NET_CONFIG_PHYSICS);
   *p++ = htonl((dword)(c->physics->airviscosity / SC_PHYSICS_VISCOUS_MAX * DWORD_MAX));
   *p++ = htonl((dword)(c->physics->gravity / SC_PHYSICS_GRAVITY_MAX * DWORD_MAX));
   *p++ = htonl((dword)(c->physics->damping / SC_TRAJ_DAMPING_MAX * DWORD_MAX));
   *p++ = htonl((dword)(c->physics->maxwind / SC_PHYSICS_WIND_MAX * DWORD_MAX));
   *p++ = htonl(c->physics->dynamicwind);
   *p++ = htonl(c->physics->suspenddirt);
   *p++ = htonl(c->physics->tanksfall);
   *p++ = htonl(c->physics->bordersextend);
   *p++ = htonl(c->physics->walls);

   *p++ = htonl(SC_NET_CONFIG_WEAPONS);
   *p++ = htonl(c->weapons->armslevel);
   *p++ = htonl(c->weapons->bombiconsize);
   *p++ = htonl(c->weapons->tunneling);
   *p++ = htonl((dword)(c->weapons->scaling / SC_WEAPON_SCALING_MAX * DWORD_MAX));
   *p++ = htonl(c->weapons->tracepaths);
   *p++ = htonl(c->weapons->uselessitems);

   *p++ = htonl(SC_NET_CONFIG_LAND);
   *p++ = htonl(c->land->sky);
   *p++ = htonl(c->land->hostileenv);
   *p++ = htonl(c->land->generator);
   *p++ = htonl((dword)(c->land->bumpiness / SC_LAND_BUMPINESS_MAX * DWORD_MAX));

   *p++ = htonl(SC_NET_CONFIG_AIC);
   *p++ = htonl(c->aicontrol->humantargets);
   *p++ = htonl(c->aicontrol->allowoffsets);
   *p++ = htonl(c->aicontrol->alwaysoffset);
   *p++ = htonl(c->aicontrol->enablescan);
   *p++ = htonl(c->aicontrol->nobudget);

   *p++ = htonl(SC_NET_CONFIG_RUNTIME);
   *p++ = htonl(randseed);
   *p++ = htonl(c->numplayers);
   if((char *)p - (char *)config.data != SC_PACKET_CONFIG_SIZE) {
      sc_net_set_error("svr_send_config", "Size mismatch on config packet");
      return(false);
   }

   /*  E N D   C O N F I G   P A C K E T  */


   /* Send data to each client connection */
   for(i = 0; i < svr->connections; ++i) {
      /* Queue config packet */
      sc_net_send_packet(&svr->clients[i], &config);

      /*  B E G I N   P L A Y E R   P A C K E T  */
      #define  SC_PACKET_PLAYER_SIZE      (SC_PLAYER_NAME_LENGTH + sizeof(dword))
      if(!sc_net_packet_init(&player, SC_NET_SVR_PLAYER_DATA, SC_PACKET_PLAYER_SIZE * SC_MAX_PLAYERS)) return(false);

      /* Setup player data packet */
      for(j = 0; j < SC_MAX_PLAYERS; ++j) {
         pc = player.data + SC_PACKET_PLAYER_SIZE * j;
         memcpy(pc, c->players[j]->name, SC_PLAYER_NAME_LENGTH);
         if(i == j) {
            *(dword *)(pc + SC_PLAYER_NAME_LENGTH) = htonl(SC_AI_HUMAN);
         } else if(c->players[j]->aitype == SC_AI_HUMAN || c->players[j]->aitype == SC_AI_NETWORK) {
            *(dword *)(pc + SC_PLAYER_NAME_LENGTH) = htonl(SC_AI_NETWORK);
         } else {
            *(dword *)(pc + SC_PLAYER_NAME_LENGTH) = htonl(c->players[j]->aitype);
         }
      }

      /*  E N D   P L A Y E R   P A C K E T  */

      /* Queue player data packet */
      sc_net_send_packet(&svr->clients[i], &player);
      sc_net_packet_release(&player);

      /* Flush the outgoing packet queue. */
      sc_net_flush_packets(&svr->clients[i]);
   }

   /* We're all done */
   sc_net_packet_release(&config);      
   sc_net_set_info("srv_send_config", "Sent game configuration");
   return(true);

}
