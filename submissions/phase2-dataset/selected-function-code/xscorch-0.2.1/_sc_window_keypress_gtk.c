static gboolean _sc_window_keypress_gtk(GtkWidget *widget, GdkEventKey *key, gpointer data) {

   sc_window_gtk *w = data;
   sc_player *curplayer = w->c->plorder[w->c->game->curplayer];
   bool controlled = key->state & GDK_CONTROL_MASK;
   bool shifted = key->state & GDK_SHIFT_MASK;

   #if SC_GTK_DEBUG_GTK && __debugging_macros
      SC_DEBUG_ENTER("%d (%s)  xx", key->keyval, gdk_keyval_name(key->keyval));
   #endif /* debug */

   /* ? */
   if(w->state < 4 && w->c->insanity &&
     !(SC_STATE_IS_ENABLED(w->c->game) &&
     !SC_STATE_IS_PAUSE(w->c->game))) {
      if(w->state == 0 && key->keyval == GDK_b)       ++w->state;
      else if(w->state == 1 && key->keyval == GDK_o)  ++w->state;
      else if(w->state == 2 && key->keyval == GDK_o)  ++w->state;
      else if(w->state == 3 && key->keyval == GDK_m)  ++w->state;
      else w->state = 0;
      if(w->state >= 4) {
         sc_status_message((sc_window *)w, "BOOM!  Heh heh heh...");
      }
   }

   /* NOTE: the g_signal_stop_emission_by_name() calls prevent GTK's
      focus manager from attempting to tab to other active controls
      on the screen; this prevents, f.e., the Tab key from simult.
      refocusing the controls and advancing weapons, which if you
      think about it really makes no sense. */

   /* Main game */
   if(SC_STATE_IS_ENABLED(w->c->game) && !SC_STATE_IS_PAUSE(w->c->game) && !controlled) {
      /* Make sure the current player is human */
      if(curplayer != NULL && curplayer->aitype == SC_AI_HUMAN) switch(key->keyval) {
         case GDK_Tab:
            sc_player_advance_weapon(w->c, curplayer, shifted ? -1 : 1);
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
         case GDK_Up:
         case GDK_KP_Up:
         case GDK_Page_Up:
            sc_player_advance_power(w->c, curplayer,
                                    shifted ? SC_PLAYER_POWER_STEP : SC_PLAYER_POWER_BIGSTEP);
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
         case GDK_Down:
         case GDK_KP_Down:
         case GDK_Page_Down:
            sc_player_advance_power(w->c, curplayer, 
                                    -(shifted ? SC_PLAYER_POWER_STEP : SC_PLAYER_POWER_BIGSTEP));
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
         case GDK_Right:
         case GDK_KP_Right:
            sc_player_advance_turret(w->c, curplayer,
                                     -(shifted ? SC_PLAYER_TURRET_STEP : SC_PLAYER_TURRET_BIGSTEP));
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
         case GDK_Left:
         case GDK_KP_Left:
            sc_player_advance_turret(w->c, curplayer,
                                     shifted ? SC_PLAYER_TURRET_STEP : SC_PLAYER_TURRET_BIGSTEP);
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
         case GDK_B:
         case GDK_b:
            sc_player_activate_battery(w->c, curplayer);
            return(TRUE);
         case GDK_E:
         case GDK_e:
            sc_player_activate_shield(w->c, curplayer);
            return(TRUE);
         case GDK_F:
         case GDK_f:
            sc_window_tank_move_gtk(w, curplayer);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_R:
         case GDK_r:
            sc_window_paint((sc_window *)w, 0, 0,
                            w->c->fieldwidth, w->c->fieldheight,
                            SC_PAINT_EVERYTHING);
            return(TRUE);
         case GDK_S:
         case GDK_s:
            sc_player_advance_shield(w->c, curplayer, SC_PLAYER_SHIELD_DEFAULTS);
            return(TRUE);
         case GDK_T:
         case GDK_t:
            sc_player_toggle_contact_triggers(w->c, curplayer);
            return(TRUE);
         case GDK_1:
         case GDK_KP_1:
            if(w->c->numplayers < 1) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[0]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_2:
         case GDK_KP_2:
            if(w->c->numplayers < 2) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[1]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_3:
         case GDK_KP_3:
            if(w->c->numplayers < 3) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[2]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_4:
         case GDK_KP_4:
            if(w->c->numplayers < 4) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[3]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_5:
         case GDK_KP_5:
            if(w->c->numplayers < 5) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[4]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_6:
         case GDK_KP_6:
            if(w->c->numplayers < 6) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[5]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_7:
         case GDK_KP_7:
            if(w->c->numplayers < 7) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[6]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_8:
         case GDK_KP_8:
            if(w->c->numplayers < 8) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[7]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_9:
         case GDK_KP_9:
            if(w->c->numplayers < 9) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[8]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_0:
         case GDK_KP_0:
            if(w->c->numplayers < 10) return(FALSE);
            sc_window_tank_info_gtk(w, w->c->players[9]);
            sc_game_pause(w->c, w->c->game);
            return(TRUE);
         case GDK_Return:
         case GDK_KP_Enter:
            sc_status_message((sc_window *)w, "");
            sc_game_set_state_asap(w->c->game, SC_STATE_TURN_PL_DONE);
            g_signal_stop_emission_by_name(G_OBJECT(widget), "key_press_event");
            return(TRUE);
      }
   }

   return(FALSE);

}
