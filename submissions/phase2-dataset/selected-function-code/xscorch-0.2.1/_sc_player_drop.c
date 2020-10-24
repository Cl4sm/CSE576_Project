static bool _sc_player_drop(sc_config *c, sc_player *p, int falldist) {
/* sc_player_drop
   Drop current player; return nonzero if still falling */

   int deltax;
   int dropdist;

   if(falldist <= 0) return(true);

   if(SC_CONFIG_NO_ANIM(c)) dropdist = c->fieldheight;
   else dropdist = falldist;

   /* Note: this function needs to use the dead flag; if the player is
      technically lifeless but the tank has not been removed from the
      playfield yet, then the tank should still take a plunge.  */
   if(!p->dead) {
      /* Get maximum height of land around and just below the tank */
      falldist = 0;
      while(falldist < dropdist && sc_player_passable(c, p, p->x, p->y - 1)) {
         /* Still falling vertically down */
         if(falldist == 0) sc_window_undraw_tank(c->window, p);
         ++falldist;
         --p->y;
      } /* Falling tank vertically down */
      if(falldist > 0) {
         sc_window_draw_tank(c->window, p);
         _sc_player_drop(c, p, dropdist - falldist);
         /* Assume need recurse, a tank may have been using us for support */
         return(true);
      } /* Haven't landed yet */

      /* We've landed */
      /* Might be sliding on the slope at this point */
      deltax = sc_land_support(c, c->land, p->x, p->y, p->tank->radius, p->tank->shelfsize);
      if(deltax != 0 && sc_player_passable(c, p, p->x + deltax, p->y - 1)) {

         /* We slid to one side (deltax > 0, slide right) */
         sc_window_undraw_tank(c->window, p);
         p->x += deltax;
         --p->y;
         sc_window_draw_tank(c->window, p);
         falldist = dropdist - 4 * abs(deltax);
         _sc_player_drop(c, p, falldist);
         /* Assume need recurse, a tank may have been using us for support */
         return(true);
      } /* Tank slid on a steep slope */
   } /* Tank isn't dead */

   /* Tank has settled onto the ground */
   return(false);

}
