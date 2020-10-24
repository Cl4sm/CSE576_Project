static sc_trajectory_result _sc_traj_traverse_roller(sc_config *c, sc_trajectory *tr, int flags,
                                                     double distance, sc_trajectory_action action,
                                                     void *data) {
/* sc_traj_traverse_roller
   This function follows a roller, from source (x,y) to its (intended)
   destination <distance> units away.  If the roller is not supported
   from below, then well, it falls.  Otherwise, if the roller has a clear
   passage to the right/left then it will continue rolling merrily along
   in a horizontal fashion.  If it cannot continue rolling (wall obstruct,
   hit a tank or other ground) then it detonates.  On return, *tr will
   contain a new position (or the detonation position).  */

   sc_trajectory_result result;/* Tracking result for this function */
   sc_trajectory_data t;   /* Data for tracking (passed to subfunctions) */
   double origx;           /* Saved value for X position */
   double origy;           /* Saved value for Y position */
   double step;            /* Current step size. */

   /* Sanity checks */
   assert(c != NULL && tr != NULL);
   assert(distance >= 0);

   /* Loop until we run out of distance to travel. */
   result = SC_TRAJ_CONTINUE;
   while(distance > 0 && result == SC_TRAJ_CONTINUE) {
      /* Setup the trajectory stepping */
      step = (distance >= 1 ? 1 : distance);
      t.stepx = step * SGN(tr->velx);
      t.stepy = 0;

      /* Save the current coordinates for later, when we start
         to experiment how we can move from this current position. */
      origx = tr->curx;
      origy = tr->cury;

      if(SC_PHYSICS_DEBUG_ROLLER) {
         printf("roller traverse %8p  at %16g %16g  land is %4d  velocity is %+g\n",
                (void *)tr, origx, origy,
                sc_land_height(c->land, rint(origx), c->land->height),
                tr->velx);
         printf("roller traverse %8p  la %16g %16g\n",
                (void *)tr, tr->lastx, tr->lasty);
      }

      /* Find out if we impacted.  The type of this initial impact
         will determine whether we detonate or roll up an incline. */
      result = _sc_traj_pass_or_hit(c, tr, flags, &t);
      if(result == SC_TRAJ_CONTINUE) {
         /* This is the easy case; we were able to continue rolling
            laterally with no difficulties.  */

         /* Check for a sign reversal, e.g. from rebound on walls. */
         if(SGN(t.stepx) != SGN(tr->velx)) {
            tr->velx = -tr->velx;
         }

         /* Weapon did not hit something; continue ... */
         if(action != NULL) {
            result = action(c, tr, data);
         } /* Is user using an action function? */

         if(result == SC_TRAJ_CONTINUE) {
            /* Attempt to move; if we are not supported, then fall.
               Otherwise, roll to right/left depending on the sign
               of the velocity term. */
            /* Set lastx, lasty now that we are certain we're moving */
            tr->lastx = origx;
            tr->lasty = origy;
            _sc_traj_roller_setup(tr, &t, origx, origy, 0, -step);
            if(!SC_TRAJ_IS_IMPACT(_sc_traj_pass_or_hit(c, tr, flags, &t))) {
               /* Falling; we already modified the position, so we can
                  leave the curx, cury alone. */
               if(SC_PHYSICS_DEBUG_ROLLER) {
                  printf("roller traverse %8p  roller is currently falling at Y %+g\n",
                         (void *)tr, -step);
               }
               tr->velx += SC_TRAJ_ROLLER_FALL * SGN(tr->velx);
               _sc_traj_roller_fallthrough(c, tr, flags);
            } else {
               /* Not falling; roll in respective direction.  We need
                  to restore curx, cury.  Note that the NEXT iteration
                  will check to see if this destination was an impact. */
               if(SC_PHYSICS_DEBUG_ROLLER) {
                  printf("roller traverse %8p  roller proceeds in X %+g\n",
                         (void *)tr, SGN(tr->velx) * step);
               }
               _sc_traj_roller_restore(tr, origx + SGN(tr->velx) * step, origy);
            } /* Which direction? */
         } /* Still going? */
      } /* Did we hit something? */

      if(result == SC_TRAJ_IMPACT_LAND) {
         /* The *current* position has impacted land.  We may attempt
            to incline by a small amount here.  */
         _sc_traj_roller_setup(tr, &t, origx, origy, 0, 1);
         if(fabs(tr->velx) > SC_TRAJ_ROLLER_GAIN &&
           !SC_TRAJ_IS_IMPACT(_sc_traj_pass_or_hit(c, tr, flags, &t))) {
            /* Salvation for the little weapon! */
            /* Set lastx, lasty now that we are certain we're moving */
            tr->lastx = origx;
            tr->lasty = origy;
            if(SC_PHYSICS_DEBUG_ROLLER) {
               printf("roller traverse %8p  roller ascended to Y %+d\n",
                  (void *)tr, 1);
            }
            tr->velx -= SC_TRAJ_ROLLER_GAIN * SGN(tr->velx);
            result = SC_TRAJ_CONTINUE;
         } else {
            /* Desperation */
            _sc_traj_roller_setup(tr, &t, origx, origy, 0, 2);
            if(fabs(tr->velx) > 2 * SC_TRAJ_ROLLER_GAIN &&
              !SC_TRAJ_IS_IMPACT(_sc_traj_pass_or_hit(c, tr, flags, &t))) {
               /* We can still salvage! */
               /* Set lastx, lasty now that we are certain we're moving */
               tr->lastx = origx;
               tr->lasty = origy;
               if(SC_PHYSICS_DEBUG_ROLLER) {
                  printf("roller traverse %8p  roller ascended to Y %+d\n",
                     (void *)tr, 2);
               }
               tr->velx += -2 * SC_TRAJ_ROLLER_GAIN * SGN(tr->velx);
               result = SC_TRAJ_CONTINUE;
            } else {
               /* Cannot recover; not enough velocity. */
               if(SC_PHYSICS_DEBUG_ROLLER) {
                  printf("roller traverse %8p  roller tried to ascend, but failed.\n",
                     (void *)tr);
               }
               _sc_traj_roller_restore(tr, origx, origy);
            }
         }
      } /* Could we recover from an impact? */

      /* Next... */
      --distance;
   } /* Loop */

   /* Return our status. */
   return(result);

}
