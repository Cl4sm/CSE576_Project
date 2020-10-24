static sc_trajectory_result _sc_traj_traverse_path(sc_config *c, sc_trajectory *tr, int flags,
                                                   sc_trajectory_action action, void *data) {
/* sc_traj_traverse_path
   This function follows a weapon, from source (x,y) to its (intended)
   destination of (x+vx,y+vy).  Compensations in velocity are automatically
   made if the weapon bounces off a wall, etc, until weapon has travelled
   for the proper amount of time.  On return, *wp will contain a new
   position (or detonation position) and a velocity.

   Note that this function WILL track subpixel movements, so that shield
   boundary detection etc. work properly.  */

   sc_trajectory_result result;/* Tracking result for this function */
   sc_trajectory_data t;   /* Data for tracking (passed to subfunctions) */
   int deltax;             /* Change in X, round up to nearest int */
   int deltay;             /* Change in Y, round up to nearest int */
   int numsteps;           /* Number of steps to take */
   int iterator;           /* Iterator variable */

   assert(c != NULL && tr != NULL);

   /* ENTERING THIS FUNCTION:
         tr->curx, tr->cury reflect the interpolation point we are
            trying to reach.  If we have a smooth flight, the weapon
            will effectively be at this interpolation point at the
            end of the function.
         tr->oldx, tr->oldy reflect the interpolation point we were
            at when we entered this function.  This is the point we
            are moving away from.
         tr->lastx, tr->lasty reflect the last coordinate (rounded
            to the nearest pixel) that was actually TESTED for an
            impact or boundary crossing.  For determining if we
            have already checked a pixel for impact (and possibly
            already cleared it), we use lastx,y.  Also, to determine
            if the weapon just crossed a shield boundary, we rely
            on lastx,y.

      DURING THIS FUNCTION:
         tr->curx, tr->cury are updated to reflect the coordinate
            that is currently being tested.  When action is called,
            curx,y will reflect the current weapon position, not the
            intended final destination.  If the weapon impacts before
            it reaches the control point, then curx,y will reflect
            the point of impact.
         tr->lastx, tr->lasty are maintained to always reflect the
            last coordinate that was actually tested.
    */

   /* Populate data in t, with initial position and "real" velocity */
   t.stepx = tr->curx - tr->oldx;
   t.stepy = tr->cury - tr->oldy;
   tr->curx = tr->oldx;
   tr->cury = tr->oldy;

   /* Determine the distance travelled along X and Y (round up,
      so worst case scenario we double-check a discrete point,
      as opposed to accidentally skipping discrete points).  */
   deltax = ceil(fabs(t.stepx));
   deltay = ceil(fabs(t.stepy));

   /* WARNING:  We can ONLY abort if there was NO step whatsoever.
      If we tracked subpixel movement (e.g. the initial and final
      coordinates round to the same pixel), we must still go through
      all the motions, otherwise we might pass through boundaries
      without realizing it... */

   /* Sanity check; if we're not moving, then abort. */
   if(deltax == 0 && deltay == 0) return(SC_TRAJ_CONTINUE);

   /* Which axis changes more?  Label that axis the "primary" axis,
      which will determine the number of steps we need to take.  */
   if(deltax > deltay) {
      /* Primary traversal axis is X */
      numsteps = deltax;
   } else {
      /* Primary traversal axis is Y */
      numsteps = deltay;
   } /* Which axis is the primary axis? */

   /* Calculate new trajectory "velocities", which are the distance to
      travel in one iterative step.  The step velocities are therefore
      not in the same units as the normal velocities.  */
   t.stepx /= numsteps;
   t.stepy /= numsteps;

   /* Track along <numsteps> points, approximating a line from (x,y)
      to (curx,cury) -- note, that this line might change if the weapon
      is deflected from its original course.  */
   result = SC_TRAJ_CONTINUE;
   iterator = numsteps;
   while(iterator > 0 && result == SC_TRAJ_CONTINUE) {
      /* Make sure curx, cury are up-to-date.  If anything bails here, then
         they will reflect the point of impact (point of contention).  The
         old behaviour was inconsistent about what curx,y would reflect; for
         hit() it would reflect the last "safe" coordinate, but for the
         action handler it'd reflect the point of impact.  */

      /* Check if we passed through any boundaries.  Note that _sc_traj_pass
         is allowed to modify the trajectory, in the case where we pass an
         elastic wall boundary.  We must always perform the boundary-pass
         check.  */
      result = _sc_traj_pass(c, tr, flags, &t);
      if(result == SC_TRAJ_CONTINUE &&
        !(rint(tr->lastx) == rint(tr->curx) && rint(tr->lasty) == rint(tr->cury))) {
         /* Check if we hit anything.  None of these calls modify the
            trajectory, so we don't need to reassign any data here.
            Note that _sc_traj_hit is NOT idempotent; as a result, we
            must call it at most once per pixel, hence the long
            test to make sure we are not actually in the same pixel.  */
         result = _sc_traj_hit(c, tr, flags, &t);
      }

      /* Well, what happened with the impact tests? */
      if(result == SC_TRAJ_CONTINUE) {
         /* Weapon did not hit something; continue ... */
         if(action != NULL) {
            result = action(c, tr, data);
         } /* Is user using an action function? */
         if(result == SC_TRAJ_CONTINUE) {
            /* If we're here, then everyone signalled all-clear. */
            tr->lastx = tr->curx;
            tr->lasty = tr->cury;
            tr->curx += t.stepx;
            tr->cury += t.stepy;
         }
      } /* Did we hit something? */
      --iterator;    /* Next please */
   } /* While we are still following along the path... */

   /* Weapon will contain the last coordinates that were tracked.
      This means if it exploded, it will contain the coordinates
      of the explosion.  Otherwise, its velocity and position will
      be the new values (before gravity is considered).  */

   /* Return our status. */
   return(result);

}
