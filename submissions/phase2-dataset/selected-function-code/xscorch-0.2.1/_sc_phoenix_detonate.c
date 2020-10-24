static sc_phoenix_result _sc_phoenix_detonate(int locate, const sc_config *c, sc_weapon **wp, sc_explosion **e) {
/* sc_phoenix_detonate
 * Detonate a weapon in place, immediately */

   double direction;

   /* DETONATE is a special phoenix type; we do not require children... */

   /* What direction should wedged blasts spread in? */
   if(locate == SC_PHOENIX_AT_TANK)
      direction = c->players[(*wp)->playerid]->turret * M_PI / 180.0;
   else
      direction = SC_EXPL_DEFAULT_DIR;

   /* Take whatever we've got and blow it sky high! */
   if(sc_expl_add(e, sc_weapon_get_explosion(c, *wp, (*wp)->tr->curx, (*wp)->tr->cury, direction)))
      return(SC_PHOENIX_DETONATE);
   else
      return(SC_PHOENIX_FAILURE);

}
