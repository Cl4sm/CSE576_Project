static inline bool _sc_land_passable_point(const sc_config *c, const sc_player *p, int x, int y) {
/* sc_land_passable_point
   Returns true if the virtual point (x, y) is passable, considering the
   land and player profiles.  If p is non-NULL, that player's profile will
   be excluded when considering profiles.  Otherwise, all profiles are
   considered.  This is an internal function that does not check sanity.  */

   int i;               /* Iterator */

   /* Check player profiles */
   for(i = 0; i < c->numplayers; ++i) {
      /* Make sure player is live, and not == p */
      if(c->players[i] != p && SC_PLAYER_IS_ALIVE(c->players[i])) {
         /* Check if player profile obscures this point */
         if(!_sc_land_passable_opponent(c, c->players[i], x, y)) return(false);
      } /* Checking player */
   } /* Loop through all players */

   /* Check land to make sure it is clear */
   if(!sc_land_translate_xy(c->land, &x, &y)) return(false);
   if(!SC_LAND_IS_SKY(*SC_LAND_XY(c->land, x, y))) return(false);

   /* Point is passable. */
   return(true);

}
