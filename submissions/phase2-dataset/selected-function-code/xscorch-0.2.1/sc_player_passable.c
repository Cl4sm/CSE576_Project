bool sc_player_passable(const sc_config *c, const sc_player *p, int x, int y) {
/* sc_player_passable
   Returns true if the player can be placed at the virtual coordinate 
   indicated.  (x, y) represents the new `center' of the tank.  To do
   this we actually check the tank profile to make sure the land is
   passable everywhere where the tank profile is opaque. */

   const unsigned char *data;
   int radius;
   int cx;
   int cy;

   if(c == NULL || p == NULL || p->tank == NULL || p->tank->data == NULL) return(false); 

   data = p->tank->data;
   radius = p->tank->radius;
   for(cy = radius; cy >= 0; --cy) {
      for(cx = radius; cx >= -radius; --cx, ++data) {
         if(*data != SC_TANK_PROFILE_CLEAR) {
            if(!sc_land_passable_point(c, p, x + cx, y + cy)) return(false);
         }
      }
   }

   return(true);

}
