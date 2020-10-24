bool sc_player_would_impact(const sc_config *c, const sc_player *p, int x, int y) {
/* sc_player_would_impact
   Determines whether the x, y coordinate indicated is atop the player,
   i.e. a weapon at that coordinate would actually hit the player.  The
   coordinate is a virtual coordinate; returns true if an impact would
   occur. */

   const unsigned char *data;
   int radius;
   int dx;
   int dy;
   
   /* Sanity checks */
   if(c == NULL || p == NULL || p->tank == NULL || p->tank->data == NULL) return(false);
   
   /* Determine where in the player tank profile we landed */
   radius = p->tank->radius;
   if(!sc_land_calculate_deltas(c->land, &dx, &dy, p->x, p->y, x, y)) return(false);
   if(dx < -radius || dx > radius) return(false);
   if(dy < 0 || dy > radius) return(false);

   /* We're in the bounding box for the profile; determine if we hit */
   data = p->tank->data + (radius - dy) * (radius + radius + 1) + (dx + radius);
   return(*data != SC_TANK_PROFILE_CLEAR);

}
