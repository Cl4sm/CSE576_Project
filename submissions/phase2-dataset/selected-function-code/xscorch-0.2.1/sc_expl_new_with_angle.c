sc_explosion *sc_expl_new_with_angle(int centerx, int centery, int radius, int force,
                                     double direction, double angular_width,
                                     int playerid, sc_explosion_type type) {
/* sc_expl_new_with_angle
   Create a new explosion, oriented at the specified direction (in radians;
   0 = right, M_PI/2 = up), and with the specified angular width (radians).
   If the angular width is 0, then direction is ignored, and normal circular
   explosions are used.  */

   sc_explosion *e;        /* A newly created explosion */

   /* Allocate and initialize the explosion */
   e = sc_expl_new(centerx, centery, radius, force, playerid, type);
   if(e == NULL) return(NULL);

   /* Setup the angular attributes */
   e->direction      = direction;
   e->angular_width  = angular_width;

   /* Return the directed explosion. */
   return(e);

}
