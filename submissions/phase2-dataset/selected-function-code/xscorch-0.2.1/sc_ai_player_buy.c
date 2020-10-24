sc_ai_result sc_ai_player_buy(const sc_config *c, sc_player *p) {
/* sc_ai_player_buy
   Buy some weapons and accessories.  */

   int budget;    /* Amount of money allocated to spend */

   budget = _sc_ai_budget(c, p);

   switch(p->ai->realaitype) {
      case SC_AI_HUMAN:
         return(SC_AI_NO_ACTION);
         break;

      case SC_AI_NETWORK:
      case SC_AI_RANDOM:
         /* No-ops */
         break;

      case SC_AI_MORON:
         _sc_ai_buy_status(c, p);
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_last_shields(c, p, &budget);
         _sc_ai_buy_last_weapons(c, p, &budget);
         break;

      case SC_AI_SHOOTER:
         _sc_ai_buy_status(c, p);
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_best_shields(c, p, &budget);
         _sc_ai_buy_shield_sappers(c, p, &budget);
         _sc_ai_buy_weapons_by_weighted_score(c, p, &budget);
         break;

      case SC_AI_SPREADER:
         _sc_ai_buy_status(c, p);
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_best_shields(c, p, &budget);
         _sc_ai_buy_weapons_by_score(c, p, &budget);
         _sc_ai_buy_shield_sappers(c, p, &budget);
         break;

      case SC_AI_CHOOSER:
         _sc_ai_buy_status(c, p);
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_best_shields(c, p, &budget);
         _sc_ai_buy_shield_sappers(c, p, &budget);
         _sc_ai_buy_precision_weapons(c, p, &budget);
         _sc_ai_buy_batteries(c, p, &budget);
         break;

      case SC_AI_CALCULATER:
         _sc_ai_buy_status(c, p);
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_best_shields(c, p, &budget);
         _sc_ai_buy_shield_sappers(c, p, &budget);
         _sc_ai_buy_precision_weapons(c, p, &budget);
         _sc_ai_buy_batteries(c, p, &budget);
         break;

      case SC_AI_ANNIHILATER:
      case SC_AI_INSANITY:
         _sc_ai_buy_status(c, p);
         budget = p->money * 0.85;
         _sc_ai_buy_contact_triggers(c, p, &budget);
         _sc_ai_buy_weapons_by_score(c, p, &budget);
         budget = p->money;
         _sc_ai_buy_auto_defense(c, p, &budget);
         _sc_ai_buy_best_shields(c, p, &budget);
         _sc_ai_buy_batteries(c, p, &budget);
         _sc_ai_buy_shield_sappers(c, p, &budget);
         break;

   }

   return(SC_AI_CONTINUE);

}
