/* sc_traj_revise_velocity
   Call this function when you want to revise the course of the weapon
   already in flight.  Assumptions:  we want to maintain current position
   but revise the current velocity.  We do NOT want to revise the time
   step value for this trajectory.  We MAY revise the initial velocity
   and initial position.

   Let cx = current X, a = acceleration (const), t = timestep (const),
   v1 represent revised init velocity, x1 represent revised init pos.
   Then to attain a new velocity nv, we want

      cx = x1 + v1 t + a t^2 / 2
      nv = v1 + a t

   This system gives us the solution

      v1 = nv - a t
      x1 = cx - nv t + a t^2 / 2

   This function should be called instead of set_velocity once the weapon
   has been launched (and has a nonzero timestep).  DO NOT MODIFY THE
   VELOCITY DIRECTLY or you will get very inconsistent tracking results.
   Be careful when doing this inside a linear interpolation (a call to
   sc_traj_traverse_path()) -- make sure stepx, stepy for the interpolation
   are somehow revised as well so we do not stray too far from the path.  */

   double curx;
   double cury;

   /* Sanity checks */
   assert(tr != NULL);
   if(tr->type != SC_TRAJ_FLIGHT) {
      fprintf(stderr, "WARNING:  does not make sense to set velocity of non-airborn trajectories!\n");
      return;
   }

   /* Rewrite initial equation parametres */
   curx = sc_traj_get_current_x(tr);   /* These functions depend */
   cury = sc_traj_get_current_y(tr);   /* on the old parametres */
   tr->velx = nvelx - tr->timestep * tr->accx;
   tr->vely = nvely + tr->timestep * tr->accy;
   tr->ctrx = curx - tr->timestep * (tr->velx + 0.5 * tr->timestep * tr->accx);
   tr->ctry = cury - tr->timestep * (tr->vely - 0.5 * tr->timestep * tr->accy);

}
