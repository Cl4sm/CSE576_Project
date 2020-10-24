/* sc_land_passable_opponent
   Returns true if the land at virtual (x, y) is passable, considering only
   p's land profile.  This is an internal function with no sanity checks.  */

   int radius;          /* Radius of player profile */
   int width;           /* Total width of player profile */
   int dx;              /* Delta x from player to x */
   int dy;              /* Delta y from player to y */

   /* Determine width of profile */
   radius = p->tank->radius;
   width = radius + radius + 1;

   /* Calculate deltas */
   if(!sc_land_calculate_deltas(c->land, &dx, &dy, x, y, p->x, p->y)) return(false);
   dx = radius + dx;
   dy = radius - dy;

   /* If delta is in profile, check if profile is opaque or not. */
   if(dx >= 0 && dx < width && dy >= 0 && dy <= radius) {
      if(*(p->tank->data + dx + width * dy) != SC_TANK_PROFILE_CLEAR) {
         /* Not passable */
         return(false);
      } /* Check profile data */
   } /* Check if in profile bounding box */

   /* Pixel is passable. */
   return(true);

}
