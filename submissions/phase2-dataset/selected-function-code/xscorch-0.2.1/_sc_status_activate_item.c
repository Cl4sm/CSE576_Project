                                         ScActiveConsoleSpot *spot,
                                         sc_window_gtk *w) {

   sc_player *curplayer = w->c->plorder[w->c->game->curplayer];

   /* Make sure the game is enabled at this point */
   if(!SC_STATE_IS_ENABLED(w->c->game) || SC_STATE_IS_PAUSE(w->c->game)) return(FALSE);

   /* What action to perform? */
   switch((long)spot->data) {
      case SC_STATUS_TOGGLE_WEAPON:
         sc_player_advance_weapon(w->c, curplayer, 1);
         return(TRUE);
      case SC_STATUS_TOGGLE_SHIELD:
         sc_player_advance_shield(w->c, curplayer, SC_PLAYER_SHIELD_DEFAULTS);
         return(TRUE);
      case SC_STATUS_TOGGLE_TRIGGER:
         sc_player_set_contact_triggers(w->c, curplayer, !curplayer->contacttriggers);
         return(TRUE);
      case SC_STATUS_ACTIVATE_SHIELD:
         sc_player_activate_shield(w->c, curplayer);
         return(TRUE);
      case SC_STATUS_ACTIVATE_BATTERY:
         sc_player_activate_battery(w->c, curplayer);
         return(TRUE);
      case SC_STATUS_MOVE_PLAYER:
         sc_window_tank_move_gtk(w, curplayer);
         return(TRUE);
   }
   return(FALSE);

}
