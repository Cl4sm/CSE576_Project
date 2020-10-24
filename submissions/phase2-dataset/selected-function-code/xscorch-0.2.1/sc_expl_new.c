sc_explosion *sc_expl_new(int centerx, int centery, int radius, int force,
                          int playerid, sc_explosion_type type) {
/* sc_expl_new
   Create a new explosion.  */

   sc_explosion *e;        /* A newly created explosion */

   /* Make sure explosion has a radius */
   if(radius <= 0) return(NULL);

   /* Allocate memory for a new explosion */
   e = (sc_explosion *)malloc(sizeof(sc_explosion));
   if(e == NULL) return(NULL);

   /* Initialise variables */
   e->centerx        = centerx;
   e->centery        = centery;
   e->direction      = 0;
   e->angular_width  = 0;
   e->radius         = radius;
   e->force          = force;
   e->playerid       = playerid;
   e->type           = type;
   e->counter        = 0;

   /* By default, no chain on this weapon */
   e->chain = NULL;
   return(e);

}
