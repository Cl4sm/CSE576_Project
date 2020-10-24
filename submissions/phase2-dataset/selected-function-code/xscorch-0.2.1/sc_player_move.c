bool sc_player_move(const sc_config *c, sc_player *p, int delta) {
/* sc_player_move
   Move the player's tank by means of consuming fuel cells.  This call
   is only used when moving by using fuel, and should not be used to
   set player position by ``forces''.  If the player does not have any
   fuel, then we will give up and not attempt the move at all.  */

   int y;      /* Height of the land at the destination */

   /* Sanity checks */
   if(c == NULL || p == NULL || delta == 0) return(false);

   /* Check that the player is in a mobile tank. */
   if(!p->tank->mobile) return(false);

   /* Check that the height is not too high a climb for a heavy tank. */
   y = sc_land_height_around(c->land, p->x + delta, c->fieldheight, p->tank->radius);
   if(y > p->y + SC_TANK_CLIMB_HEIGHT) return(false);

   /* Check that the land is otherwise ``passable'', i.e. no walls
      or other tanks in our way.  */
   if(!sc_player_passable(c, p, p->x + delta, y)) return(false);

   /* Attempt to consume fuel. If this succeeds then the move goes thru */
   if(!_sc_player_consume_fuel(c->accessories, p)) return(false);
   sc_player_set_position(c, p, p->x + delta, y);
   sc_status_update(c->window, p);
   return(true);

}
