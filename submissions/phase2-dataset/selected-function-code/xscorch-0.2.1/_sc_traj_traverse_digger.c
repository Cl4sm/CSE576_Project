static sc_trajectory_result _sc_traj_traverse_digger(sc_config *c, sc_trajectory *tr,
                                                     int flags, double distance,
                                                     sc_trajectory_action action, void *data) {
/* sc_traj_traverse_digger
   This function follows a digger, from source (x,y) to some destination
   after <distance> units of movement.  The digger moves around clearing
   land and randomly changing direction.  If it leaves the dirt for any
   reason, it fizzles.  */

/* TEMP:  Digger code is probably broken.  It was never really completed. */

   const int *gradient;    /* Sky gradient */
   int gradientflag;       /* Sky gradient flag */
   bool dither;            /* Enable dithering? */
   sc_trajectory_result result;/* Tracking result for this function */
   sc_trajectory_data t;   /* Data for tracking (passed to subfunctions) */
   double step;            /* Current step. */

   assert(c != NULL && tr != NULL);

   /* Get the sky gradient */
   gradient = sc_land_sky_index(c);
   gradientflag = sc_land_sky_flag(c);
   dither = c->graphics.gfxdither;

   result = SC_TRAJ_CONTINUE;
   while(distance > 0 && result == SC_TRAJ_CONTINUE) {
      /* Setup the trajectory stepping */
      step = (distance >= 1 ? 1 : distance);

      /* Check if we hit anything */
      _sc_traj_digger_get_direction(c, tr, &t, 0);
      result = _sc_traj_hit(c, tr, flags, &t);
      switch(result) {
      case SC_TRAJ_IMPACT_LAND:
         /* We're okay. */
         result = SC_TRAJ_CONTINUE;
         /* Call the tracking function. */
         if(action != NULL) {
            result = action(c, tr, data);
         } /* Is user using an action function? */
         if(result == SC_TRAJ_CONTINUE) {
            if(game_lrand(100) < SC_TRAJ_DIGGER_PROB) {
               /* Randomly change directions */
               _sc_traj_digger_reselect_dir(c, tr, flags);
            }
         }
         break;

      case SC_TRAJ_IMPACT_TANK:
      case SC_TRAJ_IMPACT_SHIELD:
         /* Death to the tank! */
         break;

      case SC_TRAJ_IMPACT_WALL:
      case SC_TRAJ_IMPACT_GROUND:
      case SC_TRAJ_CONTINUE:
      case SC_TRAJ_SIZZLE:
         /* We're dead. */
         result = SC_TRAJ_SIZZLE;
         break;
      }

      /* Next... */
      --distance;
   } /* Loop */

   /* Return our status. */
   return(result);

}
