void sc_traj_revise_acceleration(sc_trajectory *tr, double naccx, double naccy) {
/* sc_traj_revise_acceleration
   Call this function when you want to revise the course of the weapon
   already in flight via its acceleration terms.  Assumptions:  we want
   to maintain current position but revise the constant acceleration
   terms.  We do NOT want to revise the time step value for this trajectory.
   We MAY revise the initial velocity and initial position.

   Let cx = current X, na = new acceleration term, t = timestep (const),
   and let cv = current velocity.  Then to attain a new const acceleration
   term na, we want

      cx = x1 + v1 t + na t^2 / 2
      cv = v1 + na t

   This system gives us the solution

      v1 = cv - na t
      x1 = cx - v1 t - na t^2 / 2

   This function should be called instead of modifying acceleration terms
   directly, for FLIGHT weapons.  */

   double curx;
   double cury;
   double velx;
   double vely;

   /* Sanity checks */
   assert(tr != NULL);
   if(tr->type != SC_TRAJ_FLIGHT) {
      fprintf(stderr, "WARNING:  does not make sense to set acceleration of non-airborn trajectories!\n");
      return;
   }

   /* Rewrite initial equation parametres */
   curx = sc_traj_get_current_x(tr);   /* These functions depend */
   cury = sc_traj_get_current_y(tr);   /* on the old parametres */
   velx = sc_traj_get_velocity_x(tr);
   vely = sc_traj_get_velocity_y(tr);
   tr->velx = velx - tr->timestep * naccx;
   tr->vely = vely + tr->timestep * naccy;
   tr->ctrx = curx - tr->timestep * (tr->velx + 0.5 * tr->timestep * naccx);
   tr->ctry = cury - tr->timestep * (tr->vely - 0.5 * tr->timestep * naccy);
   tr->accx = naccx;
   tr->accy = naccy;
   /* This does not revise original accelerations */

}
