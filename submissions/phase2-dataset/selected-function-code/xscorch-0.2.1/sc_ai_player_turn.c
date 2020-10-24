/* sc_ai_player_turn
   AI player takes a turn. */

   const sc_player *target = NULL;

   assert(c != NULL && p != NULL);

   switch(p->ai->realaitype) {
      case SC_AI_HUMAN:
         return(SC_AI_NO_ACTION);
         break;

      case SC_AI_RANDOM:
      case SC_AI_NETWORK:
         /* No-ops */
         break;

      case SC_AI_MORON:
         _sc_ai_turn_status(c, p);
         _sc_ai_raise_shields(c, p);
         _sc_ai_set_contact_triggers(c, p);
         _sc_ai_recharge_tank(c, p);
         _sc_ai_select_last_weapon(c, p);
         target = _sc_ai_random_fire(c, p);
         _sc_ai_select_shield_sappers(c, p, target);
         break;

      case SC_AI_SHOOTER:
      case SC_AI_SPREADER:
         _sc_ai_turn_status(c, p);
         _sc_ai_raise_shields(c, p);
         _sc_ai_set_contact_triggers(c, p);
         _sc_ai_recharge_tank(c, p);
         _sc_ai_select_weapon_by_score(c, p);
         target = _sc_ai_calculated_fire(c, p);
         _sc_ai_select_shield_sappers(c, p, target);
         break;

      case SC_AI_CHOOSER:
         _sc_ai_turn_status(c, p);
         _sc_ai_raise_shields(c, p);
         _sc_ai_set_contact_triggers(c, p);
         _sc_ai_recharge_tank(c, p);
         _sc_ai_select_weapon_by_score(c, p);
         target = _sc_ai_fire_at_victim(c, p);
         _sc_ai_select_shield_sappers(c, p, target);
         break;

      case SC_AI_CALCULATER:
      case SC_AI_ANNIHILATER:
      case SC_AI_INSANITY:
         _sc_ai_turn_status(c, p);
         _sc_ai_raise_shields(c, p);
         _sc_ai_set_contact_triggers(c, p);
         _sc_ai_recharge_tank(c, p);
         _sc_ai_select_weapon_by_score(c, p);
         target = _sc_ai_fire_at_victim_ruthlessly(c, p);
         _sc_ai_select_shield_sappers(c, p, target);
         if(c->aicontrol->enablescan) {
            if(p->ai->victim != NULL) sc_ai_trajectory_scan(c, p, p->ai->victim);
         } /* scan refinement? */
         break;

   }

   return(SC_AI_CONTINUE);

}
