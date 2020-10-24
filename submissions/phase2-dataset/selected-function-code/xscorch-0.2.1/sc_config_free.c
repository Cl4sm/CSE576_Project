/* sc_config_free
   Release the configuration structure, and all associated substructures. */

   int i;                  /* General iterator */

   /* Sanity checks */
   if(c == NULL || *c == NULL) return;

   /* Release all substructures */
   sc_accessory_config_destroy(&(*c)->accessories);
   sc_economy_config_destroy(&(*c)->economics);
   sc_weapon_config_destroy(&(*c)->weapons);
   sc_color_free(&(*c)->colors);
   sc_game_free(&(*c)->game);
   sc_land_free(&(*c)->land);
   sc_physics_free(&(*c)->physics);
   sc_lottery_free(&(*c)->lottery);
   sc_ai_controller_free(&(*c)->aicontrol);
   sc_tank_profile_free(&(*c)->tanks);
   for(i = 0; i < SC_MAX_PLAYERS; ++i) {
      sc_player_free(&((*c)->players[i]));
   }
   reg_free(&(*c)->cfreg);
   sc_sound_free(&(*c)->sound);
   #if USE_NETWORK
      sc_net_client_free(&(*c)->client, "Client shutdown");
      sc_net_server_free(&(*c)->server, "Server shutdown");
   #endif /* Network allowed? */
   sc_registry_free(&(*c)->registry);

   /* Release the config structure */
   free(*c);
   *c = NULL;

}
