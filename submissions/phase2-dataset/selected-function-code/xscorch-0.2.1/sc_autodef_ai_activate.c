/* sc_autodef_ai_activate
   Activate an AI's Auto Defense system.
   Do NOT call game_rand from this function!  You have been warned! */

   sc_auto_def_set ads;

   /* Sanity checks. */
   if(c == NULL || p == NULL) return(false);

   /* Check for auto defense capability. */
   if(!(p->ac_state & SC_ACCESSORY_STATE_AUTO_DEF)) return(false);

   switch(p->aitype) {
      case SC_AI_ANNIHILATER:
      case SC_AI_CALCULATER:
      case SC_AI_CHOOSER:
      case SC_AI_INSANITY:
      case SC_AI_MORON:
      case SC_AI_SHOOTER:
      case SC_AI_SPREADER:
         /*
          * TEMP - current issues - JL
          * (1) All AIs use the same settings currently.
          * (2) Should this perhaps be in sai or splayer instead?
          * (3) The tracking stuff is unimplemented currently.
          * (4) Same with parachutes, and is 8 pixels good?
          */
         ads.auto_guidance = NULL;
         ads.auto_shield   = sc_shield_find_best(c, p);
         ads.chute_height  = 8;
         ads.triggers      = true;
         return(sc_autodef_activate(c, p, &ads));
      case SC_AI_HUMAN:
      case SC_AI_NETWORK:
      default:
         /* Non local AI players do not use this function. */
         return(false);
   }

}
