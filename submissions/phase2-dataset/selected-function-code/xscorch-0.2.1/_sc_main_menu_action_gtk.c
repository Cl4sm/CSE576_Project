
   switch(row) {
      case SC_ACTION_PLAYER:
         sc_player_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_ECONOMY:
         sc_economy_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_PHYSICS:
         sc_physics_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_LAND:
         sc_land_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_WEAPONS:
         sc_weapons_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_OPTIONS:
         sc_options_setup_gtk(w);
         return(TRUE);
   
      case SC_ACTION_GRAPHICS:
         sc_graphics_setup_gtk(w);
         return(TRUE);

      case SC_ACTION_AI:
         sc_ai_controller_setup_gtk(w);
         return(TRUE);

      case SC_ACTION_SOUND:
         sc_sound_setup_gtk(w);
         return(TRUE);

      case SC_ACTION_SAVE:
         sc_config_file_save_gtk(w);
         return(TRUE);
   
      case SC_ACTION_BEGIN:
         if(sc_config_okay_to_begin(w->c)) {
            sc_display_console_detach_all(SC_DISPLAY(w->screen));
            sc_game_set_state_now(w->c, w->c->game, SC_STATE_GAME_BEGIN);
         }
         return(TRUE);
   
      case SC_ACTION_QUIT:
         gtk_main_quit();
         return(TRUE);
         
      default:
         return(FALSE);
   }
   
}
