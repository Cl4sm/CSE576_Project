bool sc_preround_auto_defense(struct _sc_config *c, struct _sc_player *p) {
/* sc_preround_auto_defense
   Run the auto defense screen in pre-round mode.
   Will return true if a wait is expected. */

   /* Sanity checks. */
   if(c == NULL || p == NULL) return(false);

   /* Check for auto defense capability. */
   if(!(p->ac_state & SC_ACCESSORY_STATE_AUTO_DEF)) return(false);

   switch(p->aitype) {
      case SC_AI_NETWORK:
         /* We do not perform any action for network controlled players. */
         return(false);
      case SC_AI_HUMAN:
         /* Humans get a nice screen they can set stuff on. */
         sc_window_auto_defense(c->window, p);
         return(true);
      default:
         /* Local AIs set their auto defense parameters in realtime. */
         sc_autodef_ai_activate(c, p);
         return(false);
   }

}
